#ifndef JOYSTICK_LED_H
#define JOYSTICK_LED_H

#include <Arduino.h>

void setupHardware();
void processAxisDualDirection(const char* axisName, int joyPin, int posLedPin, int negLedPin);

#endif