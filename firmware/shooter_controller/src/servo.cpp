#include "servo.h"

bool servoHealthchecks(SCSCL sc, int id_0, int id_1)
{
    return sc.ReadTemper(id_0) < SAFE_TEMP_MAX && sc.ReadTemper(id_1) < SAFE_TEMP_MAX;
}

void servoDetails(SCSCL sc, int id, HardwareSerial serial)
{
    serial.printf("===\nID %d\n", id);
    serial.printf("pos: %d\n", sc.ReadPos(id));
    serial.printf("speed: %d\n", sc.ReadSpeed(id));
    serial.printf("temp: %dc\n", sc.ReadTemper(id));
    serial.printf("volt: %fv\n", sc.ReadVoltage(id) / 10.0);
    serial.println("---");
}

void servoMoveBothTo(SCSCL sc, int id_0, int id_1, int pos_0, int pos_1)
{
    u8 id[2];
    u16 pos[2];
    u16 times[2];

    id[0] = id_0;
    id[1] = id_1;
    pos[0] = pos_0;
    pos[1] = pos_1;
    times[0] = 10;
    times[1] = 10;

    sc.SyncWritePos(id, 2, pos, times, NULL);
}

void servoMoveTo(SCSCL sc, int id, int pos)
{
    sc.WritePos(id, pos, 10);
}

void servoEnableTorque(SCSCL sc, int id, bool enable)
{
    sc.EnableTorque(id, enable);
}

void servoArmBall(SCSCL sc, int id_0, int id_1)
{
    int id_0_pos = 1680;
    int id_1_pos = 2380;

    servoMoveBothTo(sc, id_0, id_1, id_0_pos, id_1_pos);
}

void servoLoadBall(SCSCL sc, int id_0, int id_1)
{
    int id_0_pos = 1230;
    int id_1_pos = 2855;

    servoMoveBothTo(sc, id_0, id_1, id_0_pos, id_1_pos);
}

void servoPushBall(SCSCL sc, int id_0, int id_1)
{
    int id_0_pos = 2930;
    int id_1_pos = 1160;

    servoMoveBothTo(sc, id_0, id_1, id_0_pos, id_1_pos);
}
