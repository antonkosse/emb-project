#include <Arduino.h>

#define BAUD_RATE 115200
#define EXTERNAL_RED_LED_PIN 20
#define EXTERNAL_BLUE_LED_PIN 21
#define BUTTON_PIN 40
#define BOOT_BUTTON_PIN 0

typedef enum {
  LED_MODE_OFF = 0,
  LED_MODE_ON,
  LED_MODE_BLINK_SLOW,
  LED_MODE_BLINK_FAST,
  LED_MODE_FLASH,
  LED_MODE_BREATHE
} led_blink_mode_t;

// Debounce Variables
unsigned long lastExternalDebounceTime = 0;
unsigned long lastInternalDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long longPressDuration = 1000; // 1 second in milliseconds

// Long Press Tracking Variables
unsigned long externalPressedTime = 0;
bool isExternalHeld = false;
bool longPressTriggered = false;

int lastExternalSteadyState = HIGH;
int currentExternalState;
int lastInternalSteadyState = HIGH;
int currentInternalState;

// State Tracking
led_blink_mode_t currentBlinkState = LED_MODE_OFF;

unsigned long lastBlinkUpdate = 0;
int blinkStep = 0;

void setup3() {
  Serial.begin(BAUD_RATE);
  pinMode(EXTERNAL_RED_LED_PIN, OUTPUT);
  pinMode(EXTERNAL_BLUE_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);
  
  digitalWrite(EXTERNAL_RED_LED_PIN, LOW);
  digitalWrite(EXTERNAL_BLUE_LED_PIN, LOW);
}

void checkInternalButton() {
  currentInternalState = digitalRead(BOOT_BUTTON_PIN);
  
  if (currentInternalState != lastInternalSteadyState) {
    if ((millis() - lastInternalDebounceTime) > debounceDelay) {
      lastInternalSteadyState = currentInternalState;
      if (lastInternalSteadyState == LOW) {
        Serial.println("Internal button pressed!");
        currentBlinkState = LED_MODE_BLINK_SLOW;
        blinkStep = 0;
        lastBlinkUpdate = 0; 
      }
    }
  } else {
    lastInternalDebounceTime = millis();
  }
}

void checkExternalButton() {
  currentExternalState = digitalRead(BUTTON_PIN);
  
  if (currentExternalState != lastExternalSteadyState) {
    if ((millis() - lastExternalDebounceTime) > debounceDelay) {
      lastExternalSteadyState = currentExternalState;
      
      if (lastExternalSteadyState == LOW) {
        externalPressedTime = millis();
        isExternalHeld = true;
        longPressTriggered = false;
      } 
      else {
        isExternalHeld = false;
        
        if (!longPressTriggered) {
          Serial.println("External button short click!");
          currentBlinkState = LED_MODE_BLINK_FAST;
          blinkStep = 0; 
          lastBlinkUpdate = 0;
        }
      }
    }
  } else {
    lastExternalDebounceTime = millis();
  }

  if (isExternalHeld && !longPressTriggered) {
    if (millis() - externalPressedTime >= longPressDuration) {
      Serial.println("External button long press detected! Turning OFF.");
      currentBlinkState = LED_MODE_OFF;
      longPressTriggered = true;
    }
  }
}

void updateBlinkState(unsigned long delayTime) {
  if (millis() - lastBlinkUpdate < delayTime) {
    return; 
  }
  
  lastBlinkUpdate = millis();

  switch(blinkStep) {
    case 0: case 4: case 8:
      digitalWrite(EXTERNAL_RED_LED_PIN, HIGH);
      digitalWrite(EXTERNAL_BLUE_LED_PIN, LOW);
      break;
    case 1: case 5: case 9:
      digitalWrite(EXTERNAL_RED_LED_PIN, LOW);
      digitalWrite(EXTERNAL_BLUE_LED_PIN, LOW);
      break;
    case 2: case 6: case 10:
      digitalWrite(EXTERNAL_RED_LED_PIN, LOW);
      digitalWrite(EXTERNAL_BLUE_LED_PIN, HIGH);
      break;
    case 3: case 7: case 11:
      digitalWrite(EXTERNAL_RED_LED_PIN, LOW);
      digitalWrite(EXTERNAL_BLUE_LED_PIN, LOW);
      break;
  }

  blinkStep++;
  if (blinkStep >= 12) {
    blinkStep = 0;
  }
}

void loop4() {
  checkExternalButton();
  checkInternalButton();

  if (currentBlinkState == LED_MODE_BLINK_FAST) {
    updateBlinkState(100);
  } 
  else if (currentBlinkState == LED_MODE_BLINK_SLOW) {
    updateBlinkState(1000);
  } 
  else if (currentBlinkState == LED_MODE_OFF) {
    digitalWrite(EXTERNAL_RED_LED_PIN, LOW);
    digitalWrite(EXTERNAL_BLUE_LED_PIN, LOW);
    blinkStep = 0; 
  }
}
