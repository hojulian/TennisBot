#include <Arduino.h>
#include "dshot.h"

hw_timer_t *timer = NULL;
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
	createMotor(0, &motor_0);
	createMotor(2, &motor_1);

	// Timer init, prescaler 80, divider 500 => 2khz
	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &onTimer, true);
	timerAlarmWrite(timer, 1000, true);
	timerAlarmEnable(timer);

	// ESC init
	vTaskDelay(500 / portTICK_PERIOD_MS);
	setMotorThrottle(0, &motor_0);
	setMotorThrottle(0, &motor_1);
}

void loop()
{
	int throttle = Serial.parseInt();

	if (throttle == 9999)
	{
		Serial.println("stop");
		setMotorThrottle(0, &motor_0);
		setMotorThrottle(0, &motor_1);
	}
	else if (throttle > 0)
	{
		Serial.printf("val: %d\n", throttle);
		setMotorThrottle(throttle, &motor_0);
		setMotorThrottle(throttle, &motor_1);
	}
}
