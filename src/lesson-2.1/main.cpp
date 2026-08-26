#include <Arduino.h>
#include "LedState.h"
#include "LedMode.h"
#include "Config.h"
#include "Led.h"

// Hardware instances & volatile flags
Led systemLed(Config::LED_PIN);
volatile bool buttonPressed = false;

// Interrupt Service Routine
void IRAM_ATTR buttonISR() {
    buttonPressed = true;
}

void setup() {
    Serial.begin(Config::SERIAL_BAUD_RATE);
    
    systemLed.init();
    
    pinMode(Config::BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(Config::BUTTON_PIN), buttonISR, FALLING);
}

void loop() {
    uint32_t loopStartMicros = micros();
    uint32_t currentMillis = millis();

    static LedMode currentMode = LedMode::Blinking;
    static LedState currentLedState = LedState::Off;
    
    static uint32_t lastBlinkTime = 0;
    static uint32_t lastDebounceTime = 0;
    static uint32_t loopCounter = 0;
    static uint64_t totalMicros = 0;

    // --- Task A: Button Handling ---
    if (buttonPressed) {
        if (currentMillis - lastDebounceTime > Config::DEBOUNCE_DELAY_MS) {
            switch (currentMode) {
                case LedMode::Blinking:      currentMode = LedMode::ConstantlyOn;  break;
                case LedMode::ConstantlyOn:  currentMode = LedMode::ConstantlyOff; break;
                case LedMode::ConstantlyOff: currentMode = LedMode::Blinking;      break;
            }
            lastDebounceTime = currentMillis;
        }
        buttonPressed = false;
    }

    // --- Task B: LED Control Logic ---
    switch (currentMode) {
        case LedMode::Blinking:
            if (currentMillis - lastBlinkTime >= Config::BLINK_INTERVAL_MS) {
                lastBlinkTime = currentMillis;
                currentLedState = (currentLedState == LedState::On) ? LedState::Off : LedState::On;
                systemLed.set(currentLedState);
            }
            break;
            
        case LedMode::ConstantlyOn:
            if (currentLedState != LedState::On) {
                currentLedState = LedState::On;
                systemLed.set(currentLedState);
            }
            break;
            
        case LedMode::ConstantlyOff:
            if (currentLedState != LedState::Off) {
                currentLedState = LedState::Off;
                systemLed.set(currentLedState);
            }
            break;
    }

    // --- Task C: Superloop Profiling ---
    uint32_t loopDurationMicros = micros() - loopStartMicros;
    loopCounter++;
    
    totalMicros += loopDurationMicros;
    loopCounter++;

    if (loopCounter >= Config::MEASUREMENT_ITERATIONS) {
      float averageMicros = static_cast<float>(totalMicros) / Config::MEASUREMENT_ITERATIONS;
    
      Serial.print("Average Loop Time (us): ");
      Serial.println(averageMicros, 2);

      // Reset for next batch (no memory allocation or deallocation needed)
      totalMicros = 0;
      loopCounter = 0;
    }
}