#include "config.h"
#include "joystick_led.h"

void setup() {
  setupHardware();
}

void loop() {
  processAxisDualDirection("X-Axis", PIN_JOY_X, PIN_LED_X_POS, PIN_LED_X_NEG);
  processAxisDualDirection("Y-Axis", PIN_JOY_Y, PIN_LED_Y_POS, PIN_LED_Y_NEG);
  
  // Minimal delay for loop stability
  delay(10);
}