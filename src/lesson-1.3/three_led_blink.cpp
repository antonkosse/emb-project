#include <Arduino.h>

#define BAUD_RATE 115200
#define DELAY_MS 1000
#define EXTERNAL_RED_LED_PIN 20
#define EXTERNAL_WHITE_LED_PIN 21
#define EXTERNAL_BLUE_LED_PIN 47

void setup1() {
  Serial.begin(BAUD_RATE);
  pinMode(EXTERNAL_RED_LED_PIN, OUTPUT);
  pinMode(EXTERNAL_WHITE_LED_PIN, OUTPUT);
  pinMode(EXTERNAL_BLUE_LED_PIN, OUTPUT);
  digitalWrite(EXTERNAL_RED_LED_PIN, LOW);
  digitalWrite(EXTERNAL_WHITE_LED_PIN, LOW);
  digitalWrite(EXTERNAL_BLUE_LED_PIN, LOW);
}

void loop1() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(EXTERNAL_BLUE_LED_PIN, HIGH); delay(50);
    digitalWrite(EXTERNAL_BLUE_LED_PIN, LOW);  delay(50);
  }
  
  digitalWrite(EXTERNAL_WHITE_LED_PIN, HIGH); delay(40);
  digitalWrite(EXTERNAL_WHITE_LED_PIN, LOW);  delay(60);

  for (int i = 0; i < 3; i++) {
    digitalWrite(EXTERNAL_RED_LED_PIN, HIGH); delay(50);
    digitalWrite(EXTERNAL_RED_LED_PIN, LOW);  delay(50);
  }

  digitalWrite(EXTERNAL_WHITE_LED_PIN, HIGH); delay(40);
  digitalWrite(EXTERNAL_WHITE_LED_PIN, LOW);  delay(150); // Более длинная пауза перед циклом
}