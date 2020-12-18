#include "esp32-hal.h"
#include <inttypes.h>
#include <SoftwareSerial.h>

#define DSHOT_MIN 48
#define DSHOT_MAX 2047
#define DSHOT_IDLE 300

typedef struct
{
	rmt_data_t dshot_packet[16];
	rmt_obj_t *rmt_send;
	SoftwareSerial *p_serial;

	volatile uint16_t throttle;
} dshot_t;

void dshotCreateMotor(dshot_t *motor, int pin, SoftwareSerial *p_serial);
void dshotSetThrottle(dshot_t *motor, uint16_t throttle);
void dshotReceiveTelemetry(dshot_t *motor, uint16_t *telemetrie);
void dshotOutput(dshot_t *motor);

uint8_t _update_crc8(uint8_t crc, uint8_t crc_seed);
uint8_t _get_crc8(uint8_t *buf, uint8_t buf_len);
