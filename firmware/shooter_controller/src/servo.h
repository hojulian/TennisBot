#include <Arduino.h>
#include "SCSCL.h"

#define SAFE_TEMP_MAX 50

bool servoHealthchecks(SCSCL sc, int id_0, int id_1);
void servoDetails(SCSCL sc, int id, HardwareSerial serial);
void servoMoveBothTo(SCSCL sc, int id_0, int id_1, int pos_0, int pos_1);
void servoMoveTo(SCSCL sc, int id, int pos);
void servoEnableTorque(SCSCL sc, int id, bool enable);

void servoArmBall(SCSCL sc, int id_0, int id_1);
void servoLoadBall(SCSCL sc, int id_0, int id_1);
void servoPushBall(SCSCL sc, int id_0, int id_1);
