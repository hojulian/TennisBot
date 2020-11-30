#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp32-hal.h"
#include <inttypes.h>
#include "dshot.h"

#define MOTOR_0 0
#define MOTOR_1 1

hw_timer_t *timer = NULL;
dshot_t motors[2];

void IRAM_ATTR onTimer()
{
	dshotOutput(motors[MOTOR_0].throttle, motors[MOTOR_0].dshot_packet, motors[MOTOR_0].rmt_send);
	dshotOutput(motors[MOTOR_1].throttle, motors[MOTOR_1].dshot_packet, motors[MOTOR_1].rmt_send);
}

void createMotor(int pin, dshot_t *motor)
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
}

void setMotorThrottle(int id, uint16_t throttle) {
	if (throttle > 2000 || throttle < 0)
    {
        return;
    }

	motors[id].throttle = throttle + DSHOT_MIN;
}

void setup()
{
	Serial.begin(115200);
	Serial.println("init starting");

	// Motors init
	createMotor(0, &motors[0]);
	createMotor(2, &motors[1]);

	// Timer init, prescaler 80, divider 500 => 2khz
	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &onTimer, true);
	timerAlarmWrite(timer, 1000, true);
	timerAlarmEnable(timer);

	// ESC init
	vTaskDelay(500 / portTICK_PERIOD_MS);
	setMotorThrottle(MOTOR_0, 0);
	setMotorThrottle(MOTOR_1, 0);
}

void loop()
{
	int throttle = Serial.parseInt();
	if (throttle > 0)
	{
		Serial.printf("val: %d\n", throttle);
		setMotorThrottle(MOTOR_0, throttle);
		setMotorThrottle(MOTOR_1, throttle);
	} else if (throttle == -1)
	{
		Serial.println("stop");
		setMotorThrottle(MOTOR_0, 0);
		setMotorThrottle(MOTOR_1, 0);
	}
}
