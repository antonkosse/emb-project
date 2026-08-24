#include <Arduino.h>

#define BAUD_RATE 115200
#define INTERVAL 1000
#define EXTERNAL_LED_PIN 20

unsigned long previousMillis = 0;
bool pinState = false;

void setup2() {
  Serial.begin(BAUD_RATE);
  pinMode(EXTERNAL_LED_PIN, OUTPUT);
  digitalWrite(EXTERNAL_LED_PIN, LOW);
}

void loop2() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= INTERVAL) {
    previousMillis = currentMillis;
    
    pinState = !pinState;
    
    if (pinState) {
      digitalWrite(EXTERNAL_LED_PIN, HIGH);
    } else {
      digitalWrite(EXTERNAL_LED_PIN, LOW);
    }
  }
}