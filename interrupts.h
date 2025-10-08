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