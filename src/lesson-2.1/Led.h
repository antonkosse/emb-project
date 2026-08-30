#pragma once

#include <Arduino.h>
#include "LedState.h"

class Led {
private:
    uint8_t pin;

public:
    explicit Led(uint8_t pinNumber);
    void init() const;
    void set(LedState state) const;
};