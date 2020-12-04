#include <Arduino.h>
#include "DShot.h"

#define MOTOR_0_PIN 0
#define MOTOR_1_PIN 2
hw_timer_t *motor_timer = NULL;
dshot_t motor_0;
dshot_t motor_1;

void IRAM_ATTR onTimer()
{
	dshotOutput(motor_0.throttle, motor_0.dshot_packet, motor_0.rmt_send);
	dshotOutput(motor_1.throttle, motor_1.dshot_packet, motor_1.rmt_send);
}

void setup()
{
	Serial.begin(115200);
	Serial.println("init starting");

	// Motors init
	dshotCreateMotor(MOTOR_0_PIN, &motor_0);
	dshotCreateMotor(MOTOR_1_PIN, &motor_1);

	// Timer init, prescaler 80, divider 500 => 2khz
	motor_timer = timerBegin(0, 80, true);
	timerAttachInterrupt(motor_timer, &onTimer, true);
	timerAlarmWrite(motor_timer, 1000, true);
	timerAlarmEnable(motor_timer);

	// ESC init
	vTaskDelay(500 / portTICK_PERIOD_MS);
	dshotSetThrottle(0, &motor_0);
	dshotSetThrottle(0, &motor_1);
}

void loop()
{
	int throttle = Serial.parseInt();

	if (throttle == 9999)
	{
		Serial.println("stop");
		dshotSetThrottle(0, &motor_0);
		dshotSetThrottle(0, &motor_1);
	}
	else if (throttle > 0)
	{
		Serial.printf("val: %d\n", throttle);
		dshotSetThrottle(throttle, &motor_0);
		dshotSetThrottle(throttle, &motor_1);
	}
}
