#include "joystick_led.h"
#include "config.h"

// Struct to store previous output states for change detection
struct AxisState {
  int prevPosPwm = -1;
  int prevNegPwm = -1;
};

static AxisState xState;
static AxisState yState;

void setupHardware() {
  Serial.begin(115200);
  
  pinMode(PIN_LED_X_POS, OUTPUT);
  pinMode(PIN_LED_X_NEG, OUTPUT);
  pinMode(PIN_LED_Y_POS, OUTPUT);
  pinMode(PIN_LED_Y_NEG, OUTPUT);
  
  pinMode(PIN_JOY_X, INPUT);
  pinMode(PIN_JOY_Y, INPUT);
}

void processAxisDualDirection(const char* axisName, int joyPin, int posLedPin, int negLedPin) {
  int rawValue = analogRead(joyPin);
  int signedOffset = rawValue - JOY_CENTER;

  int posPwm = 0;
  int negPwm = 0;

  if (signedOffset > DEADZONE) {
    posPwm = map(signedOffset, DEADZONE, JOY_CENTER, 0, PWM_MAX);
    posPwm = constrain(posPwm, 0, PWM_MAX);
  } 
  else if (signedOffset < -DEADZONE) {
    negPwm = map(abs(signedOffset), DEADZONE, JOY_CENTER, 0, PWM_MAX);
    negPwm = constrain(negPwm, 0, PWM_MAX);
  }

  // Write hardware PWM outputs
  analogWrite(posLedPin, posPwm);
  analogWrite(negLedPin, negPwm);

  // Select state tracker based on input pin
  AxisState* state = (joyPin == PIN_JOY_X) ? &xState : &yState;

  // Print ONLY if output PWM values have changed
  if (posPwm != state->prevPosPwm || negPwm != state->prevNegPwm) {
    Serial.printf("[%s] Raw: %4d | Offset: %5d | Pos PWM: %3d | Neg PWM: %3d\n", 
                  axisName, rawValue, signedOffset, posPwm, negPwm);
    
    // Save current states
    state->prevPosPwm = posPwm;
    state->prevNegPwm = negPwm;
  }
}