// STM32L432KC_GPIO.h
// Header for GPIO functions

// Provided by Josh Brake as part of Lab 4 Starter Code

#ifndef STM32L4_GPIO_H
#define STM32L4_GPIO_H

#include <stdint.h> // Include stdint header
#include <stm32l432xx.h>

#define GPIO_INPUT 0
#define GPIO_OUTPUT 1
#define GPIO_ALT 2
#define GPIO_ANALOG 3

#define GPIO GPIOA

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void pinMode(int pin, int function);
int digitalRead(int pin);
void digitalWrite(int pin, int val);
void togglePin(int pin);
void enablePullUp(int pin);

#endif