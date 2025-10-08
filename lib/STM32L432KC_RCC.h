// STM32L432KC_RCC.h
// Header for RCC functions

// Provided by Josh Brake as part of Lab 4 Starter Code

#ifndef STM32L4_RCC_H
#define STM32L4_RCC_H

#include <stdint.h>
#include <stm32l432xx.h>

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void configurePLL(void);
void configureClock(void);
void enableClkGPIO(void);

#endif