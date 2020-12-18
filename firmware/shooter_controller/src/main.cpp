#include <Arduino.h>
#include <SoftwareSerial.h>
#include "DShot.h"
#include "SCSCL.h"

#include "servo.h"
#include "control.h"

#define MOTOR_POLES 14
#define TOP_MOTOR_PIN 32
#define BOT_MOTOR_PIN 33
hw_timer_t *motor_timer = NULL;
dshot_t top_motor;
dshot_t bot_motor;

#define TOP_MOTOR_TELE_RX_PIN 25
#define BOT_MOTOR_TELE_RX_PIN 26
hw_timer_t *motor_tele_timer = NULL;
SoftwareSerial top_motor_tele;
SoftwareSerial bot_motor_tele;
uint16_t top_telemetrie[5];
uint16_t bot_telemetrie[5];

#define TOP_SERVO_ID 1
#define BOT_SERVO_ID 2
#define SERVO_RX_PIN 25
#define SERVO_TX_PIN 27
HardwareSerial p_serial(2);
SCSCL sc(0);

hw_timer_t *display_timer = NULL;

void IRAM_ATTR onTeleTimer()
{
	// dshotReceiveTelemetry(&top_motor, top_telemetrie);
	// dshotReceiveTelemetry(&bot_motor, bot_telemetrie);
}

void IRAM_ATTR onMotorTimer()
{
	dshotOutput(&top_motor);
	dshotOutput(&bot_motor);
}

void IRAM_ATTR display()
{
	// Serial.println("----------");
	// Serial.printf("top:\n %dc, %fv, %fA, %drpm\n",
	// 			  top_telemetrie[0],
	// 			  top_telemetrie[1] / 100.0,
	// 			  top_telemetrie[2] / 10.0,
	// 			  top_telemetrie[4] * 100 / (MOTOR_POLES / 2));

	// Serial.printf("\nbot:\n %dc, %fv, %fA, %drpm\n",
	// 			  bot_telemetrie[0],
	// 			  bot_telemetrie[1] / 100.0,
	// 			  bot_telemetrie[2] / 10.0,
	// 			  bot_telemetrie[4] * 100 / (MOTOR_POLES / 2));
	// Serial.println("----------");
	
	// servoDetails(sc, TOP_SERVO_ID, Serial);
	// servoDetails(sc, BOT_SERVO_ID, Serial);
}

void setup()
{
	Serial.begin(115200);
	Serial.setTimeout(5000);
	Serial.println("init starting");

	// Servo init
	p_serial.begin(1000000, SERIAL_8N1, 25, 27);
	sc.pSerial = &p_serial;

	// Motor telemetry init
	// top_motor_tele.begin(115200, SWSERIAL_8N1, TOP_MOTOR_TELE_RX_PIN, TOP_MOTOR_TELE_RX_PIN, false, 256);
	// top_motor_tele.enableIntTx(false);
	// top_motor_tele.enableTx(false);
	// bot_motor_tele.begin(115200, SWSERIAL_8N1, BOT_MOTOR_TELE_RX_PIN, BOT_MOTOR_TELE_RX_PIN, false, 256);
	// bot_motor_tele.enableIntTx(false);
	// bot_motor_tele.enableTx(false);

	// Motors init
	dshotCreateMotor(&top_motor, TOP_MOTOR_PIN, &top_motor_tele);
	dshotCreateMotor(&bot_motor, BOT_MOTOR_PIN, &bot_motor_tele);

	// Motor timer init
	motor_timer = timerBegin(0, 80, true);
	timerAttachInterrupt(motor_timer, &onMotorTimer, true);
	timerAlarmWrite(motor_timer, 1000, true); // 1ms
	timerAlarmEnable(motor_timer);

	// Motor telemetry timer init
	motor_tele_timer = timerBegin(2, 80, true);
	timerAttachInterrupt(motor_tele_timer, &onTeleTimer, true);
	timerAlarmWrite(motor_tele_timer, 40000, true); // 20ms
	timerAlarmEnable(motor_tele_timer);

	// ESC init
	vTaskDelay(500 / portTICK_PERIOD_MS);
	dshotSetThrottle(&top_motor, 0);
	dshotSetThrottle(&bot_motor, 0);

	// Display timer init
	display_timer = timerBegin(3, 80, true);
	timerAttachInterrupt(display_timer, &display, true);
	timerAlarmWrite(display_timer, 1000000, true); // 1000ms
	timerAlarmEnable(display_timer);
}

void loop()
{
	controlPrintPrompt(Serial);

	int input = Serial.parseInt();

	switch (input)
	{
	case CONTROL_MOTOR_STOP:
	{
		Serial.println("stop");
		dshotSetThrottle(&top_motor, 0);
		dshotSetThrottle(&bot_motor, 0);
		break;
	}

	case 1:
	{
		Serial.println("10% speed");
		dshotSetThrottle(&top_motor, 200);
		dshotSetThrottle(&bot_motor, 200);
		break;
	}

	case 2:
	{
		Serial.println("20% speed");
		dshotSetThrottle(&top_motor, 400);
		dshotSetThrottle(&bot_motor, 400);
		break;
	}

	case 3:
	{
		Serial.println("30% speed");
		dshotSetThrottle(&top_motor, 600);
		dshotSetThrottle(&bot_motor, 600);
		break;

	}

	case CONTROL_MOTOR_CUSTOM_SPEED:
	{
		Serial.println("Custom speed");
		int top_rpm = Serial.parseInt();
		int bot_rpm = Serial.parseInt();

		Serial.printf("Speeding up to: %d %d\n", top_rpm, bot_rpm);
		
		dshotSetThrottle(&top_motor, top_rpm);
		dshotSetThrottle(&bot_motor, bot_rpm);
		break;
	}

	case CONTROL_SERVO_ARM:
	{
		Serial.println("Arm");
		servoArmBall(sc, TOP_SERVO_ID, BOT_SERVO_ID);
		vTaskDelay(2000 / portTICK_PERIOD_MS);
		break;
	}

	case CONTROL_SERVO_PUSH:
	{
		Serial.println("Push and Arm");
		servoLoadBall(sc, TOP_SERVO_ID, BOT_SERVO_ID);
		vTaskDelay(500 / portTICK_PERIOD_MS);

		servoPushBall(sc, TOP_SERVO_ID, BOT_SERVO_ID);
		vTaskDelay(800 / portTICK_PERIOD_MS);

		servoArmBall(sc, TOP_SERVO_ID, BOT_SERVO_ID);
		break;
	}
	
	default:
		break;
	}
}
