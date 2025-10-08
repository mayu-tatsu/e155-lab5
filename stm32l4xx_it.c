/**
* @file stm32l4xx_it.c
* @brief Interrupt handlers for the quadrature encoder (TIM2)
*/


#include "stm32l432xx.h"
#include <stdint.h>


// This global variable holds the total encoder count (32-bit total position)
volatile int32_t total_encoder_pulses = 0;


/**
* @brief TIM2 Interrupt Request Handler (handles encoder overflow/underflow)
*
* This function increments or decrements the total_encoder_pulses variable by
* the timer's period (0x10000 = 65536) every time the hardware counter rolls over.
*/
void TIM2_IRQHandler(void) {
   // Check if the Update Interrupt Flag (UIF) is set
   if (TIM2->SR & TIM_SR_UIF) {
       // Clear the Update Interrupt Flag (VERY IMPORTANT!)
       TIM2->SR &= ~TIM_SR_UIF;


       // The Direction bit (DIR) in TIMx->CR1 indicates the direction of the last count.
       // It is set (1) for down-counting and reset (0) for up-counting.


       if (TIM2->CR1 & TIM_CR1_DIR) {
           // Direction is Down (DIR=1). This means an underflow occurred
           // (Count rolled over from 0 to 0xFFFF). We subtract the period.
           total_encoder_pulses -= 0x10000;
       } else {
           // Direction is Up (DIR=0). This means an overflow occurred
           // (Count rolled over from 0xFFFF to 0). We add the period.
           total_encoder_pulses += 0x10000;
       }
   }
}


// ----------------------------------------------------------------------------
// OPTIONAL: Index Pulse EXTI Interrupt Handler (Example using PB5)
// ----------------------------------------------------------------------------
/*
// This code is conceptual. You would need to initialize PB5 as an Input,
// configure the EXTI line for falling/rising edge trigger, and enable
// the EXTI line in the NVIC before this handler is useful.


volatile uint32_t revolution_count = 0;


void EXTI9_5_IRQHandler(void) {
   // Check if the interrupt for EXTI Line 5 is pending
   if (EXTI->PR1 & EXTI_PR1_PIF5) {
       // Clear the pending bit
       EXTI->PR1 |= EXTI_PR1_PIF5;


       // Action: The index pulse indicates one full revolution
       revolution_count++;


       // You might also reset the hardware counter to ensure alignment:
       // TIM2->CNT = 0;
   }
}
*/


