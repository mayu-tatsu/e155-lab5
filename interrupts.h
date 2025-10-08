// interrupts.c
// Mayu Tatsumi; mtatsumi@g.hmc.edu
// 2025-10-7

// Implements functions for initializing and handling external
// interrupts. They trigger on falling or rising edges and
// record timestamps accordingly. Also, logic for updating
// velocity and direction were implemented using those timestamps.

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>
#include <stm32l432xx.h>
#include <system_stm32l4xx.h>

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void initInterrupt(int pin_num);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
float update_velocity(int loop_delay);
float update_direction(void);

#endif