#include <Arduino.h>
#include "DShot.h"

void dshotCreateMotor(dshot_t *motor, int pin, SoftwareSerial *p_serial)
{
	rmt_obj_t *rmt_send;
	if ((rmt_send = rmtInit(pin, true, RMT_MEM_64)) == NULL)
	{
		Serial.println("init sender failed\n");
		return;
	}

	rmtSetTick(rmt_send, 12.5);

	motor->rmt_send = rmt_send;
	motor->throttle = DSHOT_IDLE;
	motor->p_serial = p_serial;
}

void dshotSetThrottle(dshot_t *motor, uint16_t throttle)
{
	if (throttle > 2000 || throttle < 0)
	{
		return;
	}

	motor->throttle = throttle + DSHOT_MIN;
}

void dshotReceiveTelemetry(dshot_t *motor, uint16_t *telemetrie)
{
	static uint8_t serial_buf[10];
	uint8_t received = 0;

	motor->p_serial->listen();

	while (motor->p_serial->available())
	{
		serial_buf[received] = motor->p_serial->read();
		received++;

		if (received > 9)
		{
			uint8_t crc8 = _get_crc8(serial_buf, 9);

			if (crc8 != serial_buf[9])
			{
				while (motor->p_serial->available())
				{
					motor->p_serial->read();
				}

				return;
			}

			Serial.println("checksum OK");

			telemetrie[0] = serial_buf[0];						  // temperature
			telemetrie[1] = (serial_buf[1] << 8) | serial_buf[2]; // voltage
			telemetrie[2] = (serial_buf[3] << 8) | serial_buf[4]; // current
			telemetrie[3] = (serial_buf[5] << 8) | serial_buf[6]; // used mA/h
			telemetrie[4] = (serial_buf[7] << 8) | serial_buf[8]; // eRpM *100

			Serial.println("received telemetry");
			return;
		}
	}
}

uint8_t _update_crc8(uint8_t crc, uint8_t crc_seed)
{
	uint8_t crc_u, i;
	crc_u = crc;
	crc_u ^= crc_seed;
	for (i = 0; i < 8; i++)
		crc_u = (crc_u & 0x80) ? 0x7 ^ (crc_u << 1) : (crc_u << 1);
	return (crc_u);
}

uint8_t _get_crc8(uint8_t *buf, uint8_t buf_len)
{
	uint8_t crc = 0, i;
	for (i = 0; i < buf_len; i++)
		crc = _update_crc8(buf[i], crc);
	return (crc);
}

void dshotOutput(dshot_t *motor)
{
	uint16_t packet;

	// telemetry bit
	packet = (motor->throttle << 1) | 1;
	// packet = (motor->throttle << 1) | 0;

	// https://github.com/betaflight/betaflight/blob/09b52975fbd8f6fcccb22228745d1548b8c3daab/src/main/drivers/pwm_output.c#L523
	int csum = 0;
	int csum_data = packet;
	for (int i = 0; i < 3; i++)
	{
		csum ^= csum_data;
		csum_data >>= 4;
	}
	csum &= 0xf;
	packet = (packet << 4) | csum;

	// durations are for dshot600
	// https://blck.mn/2016/11/dshot-the-new-kid-on-the-block/
	// Bit length (total timing period) is 1.67 microseconds (T0H + T0L or T1H + T1L).
	// For a bit to be 1, the pulse width is 1250 nanoseconds (T1H – time the pulse is high for a bit value of ONE)
	// For a bit to be 0, the pulse width is 625 nanoseconds (T0H – time the pulse is high for a bit value of ZERO)
	for (int i = 0; i < 16; i++)
	{
		if (packet & 0x8000)
		{
			motor->dshot_packet[i].level0 = 1;
			motor->dshot_packet[i].duration0 = 100;
			motor->dshot_packet[i].level1 = 0;
			motor->dshot_packet[i].duration1 = 34;
		}
		else
		{
			motor->dshot_packet[i].level0 = 1;
			motor->dshot_packet[i].duration0 = 50;
			motor->dshot_packet[i].level1 = 0;
			motor->dshot_packet[i].duration1 = 84;
		}
		packet <<= 1;
	}

	rmtWrite(motor->rmt_send, motor->dshot_packet, 16);

	return;
}
