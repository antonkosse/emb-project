#include <Arduino.h>

const int PIN_JOY_X = 4;
const int PIN_JOY_Y = 5;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_JOY_X, INPUT);
  pinMode(PIN_JOY_Y, INPUT);
}

void loop() {
  int rawX = analogRead(PIN_JOY_X);
  int rawY = analogRead(PIN_JOY_Y);

  Serial.printf("RAW X (GPIO4): %4d | RAW Y (GPIO5): %4d\n", rawX, rawY);
  delay(200);
}