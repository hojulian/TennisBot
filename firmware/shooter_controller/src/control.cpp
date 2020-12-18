#include "control.h"

void controlPrintPrompt(HardwareSerial serial)
{
    serial.println("---- MENU ----");
    serial.println("7: stop");
    serial.println("1: 10% speed");
    serial.println("2: 20% speed");
    serial.println("3: 30% speed");
    serial.println("4: custom speed");
    serial.println("5: Arm");
    serial.println("6: Push and Arm");
    serial.println("");
}
