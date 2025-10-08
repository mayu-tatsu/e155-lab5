// interrupts.h
// Mayu Tatsumi; mtatsumi@g.hmc.edu
// 2025-10-7

// Created function containers for initializing and handling external
// interrupts, along with updating the velocity and direction of the motor.

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
float update_velocity(void);

#endif