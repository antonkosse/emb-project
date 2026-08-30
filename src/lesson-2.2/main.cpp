#include <Arduino.h>

const int RELAY_CTRL_PIN = 4;
const int RELAY_READ_PIN = 8;

volatile bool relayClosed = false;
volatile unsigned long interruptTime = 0;
unsigned long triggerTime = 0;

int measurementCount = 0;
const int MAX_MEASUREMENTS = 10;
float totalTimeMs = 0;

void IRAM_ATTR relayISR() {
  interruptTime = micros(); 
  relayClosed = true;
}

void setup() {
  Serial.begin(115200);
  
  pinMode(RELAY_CTRL_PIN, OUTPUT);
  // ACTIVE LOW FIX: Start HIGH so the relay is OFF by default
  digitalWrite(RELAY_CTRL_PIN, HIGH); 

  pinMode(RELAY_READ_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RELAY_READ_PIN), relayISR, FALLING);
  
  Serial.println("Starting Relay Activation Time Test...");
  delay(2000);
}

void loop() {
  if (measurementCount < MAX_MEASUREMENTS) {
    relayClosed = false; 
    
    // 1. Start the timer and trigger the relay (Pull LOW to turn ON)
    triggerTime = micros(); 
    digitalWrite(RELAY_CTRL_PIN, LOW); 
    
    // 2. Wait for the interrupt 
    unsigned long waitStart = millis();
    while (!relayClosed && (millis() - waitStart < 50)) {
    }
    
    // 3. Calculate result
    if (relayClosed) {
      float durationMs = (interruptTime - triggerTime) / 1000.0;
      totalTimeMs += durationMs;
      measurementCount++;
      
      Serial.print("Measurement ");
      Serial.print(measurementCount);
      Serial.print(": ");
      Serial.print(durationMs);
      Serial.println(" ms");
    } else {
      Serial.println("Timeout: Relay did not close.");
    }
    
    // 4. Turn relay OFF (Pull HIGH) and wait before next test
    digitalWrite(RELAY_CTRL_PIN, HIGH);
    delay(1000); 
    
  } else if (measurementCount == MAX_MEASUREMENTS) {
    float averageMs = totalTimeMs / MAX_MEASUREMENTS;
    Serial.println("-------------------------");
    Serial.print("Test Complete. Average Activation Time: ");
    Serial.print(averageMs);
    Serial.println(" ms");
    
    measurementCount++; 
  }
}