#ifndef CONFIG_H
#define CONFIG_H

const int PIN_JOY_X = 4;
const int PIN_JOY_Y = 5;

// 2 LEDs per axis (Directional)
const int PIN_LED_X_POS = 17; // Right
const int PIN_LED_X_NEG = 15; // Left
const int PIN_LED_Y_POS = 16; // Up
const int PIN_LED_Y_NEG = 18; // Down

const int JOY_CENTER = 2048; 
const int DEADZONE = 150;     // Ignore small jitter near rest
const int PWM_MAX = 255;

#endif