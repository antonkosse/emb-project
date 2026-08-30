#pragma once
#include <cstdint>

class Config {
public:
    static constexpr uint8_t LED_PIN = 18;            // Built-in RGB LED GPIO on ESP32-S3 DevKitC-1 is often 48
    static constexpr uint8_t BUTTON_PIN = 0;         // BOOT button
    static constexpr uint32_t BLINK_INTERVAL_MS = 500;
    static constexpr uint32_t SERIAL_BAUD_RATE = 115200;
    static constexpr uint32_t MEASUREMENT_ITERATIONS = 1000;
    static constexpr uint32_t DEBOUNCE_DELAY_MS = 50; 
};