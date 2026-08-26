#include "Led.h"

Led::Led(uint8_t pinNumber) : pin(pinNumber) {}

void Led::init() const {
    pinMode(pin, OUTPUT);
    set(LedState::Off);
}

void Led::set(LedState state) const {
    digitalWrite(pin, state == LedState::On ? HIGH : LOW);
}