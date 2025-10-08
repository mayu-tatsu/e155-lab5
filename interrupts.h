// interrupts.h
// Mayu Tatsumi; mtatsumi@g.hmc.edu
// 2025-10-7

// Header file for interrupts.c. Defines function prototypes
// for initializing and handling external interrupts. Also
// logic for updating velocity and direction based on encoder signals.

#ifndef STM32L4_INTERRUPT_H
#define STM32L4_INTERRUPT_H

#include <stdint.h>
#include <stm32l432xx.h>
#include <system_stm32l4xx.h>

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void initInterrupt(int pin_num);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler();
float update_velocity(int loop_delay);
float update_direction(void);

#endif