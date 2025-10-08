// STM32L432KC_TIM1516.c
// Mayu Tatsumi; mtatsumi@g.hmc.edu
// 2025-9-30

// Defines all functions required for playing the speaker,
// building off the _TIM1516 header file. Has functions for
// the instantiation for both pins 15 and 16. 15 functions
// just as a timer, and 16 as a PWM. delay_ms creates a
// delay via while loop with nothing in it, taking timer 15.
// speaker_freq adjusts the frequency of each note for the
// PWM output from timer 16.

#include "STM32L432KC_TIM1516.h"

void initTIM1516(TIM_TypeDef* TIMX) {
  TIMX->PSC = DELAY_PSC;      // Set prescaler
  
  // Disabling slave mode
  TIMX->SMCR &= ~(    1 << 16);
  TIMX->SMCR &= ~(0b111 << 0);

  TIMX->ARR = 0xFFFF;         // Max ARR to ensure no event spamming
  TIMX->EGR |= (1 << 0);      // Generates event to reinitialize counter
  TIMX->CR1 |= (1 << 0);      // Enable counter
  TIMX->CNT = 0;              // Reset counter to 0
}

void initTIM1516_PWM(TIM_TypeDef* TIMX) {
  TIMX->PSC = FREQ_PSC;      // Set prescaler

  TIMX->CCR1 = 0x7FFF;            // Using CH1, set 50% duty cycle
  TIMX->CCMR1 &= ~(0b111 << 4);   // Cleaning
  TIMX->CCMR1 |=  (0b110 << 4);   // Set PWM mode 1
  TIMX->CCMR1 |=  (1     << 3);   // Enable CH1 preload register

  TIMX->BDTR |= (1 << 15);        // Main output enable

  TIMX->CCER |=  (1 << 0);     // CC1E output enable
  TIMX->CCER &= ~(1 << 1);     // CC1P output polarity

  TIMX->ARR  = 0xFFFF;        // Max ARR to ensure no event spamming
  TIMX->EGR |= (1 << 0);      // Generates event to reinitialize counter
  TIMX->CR1 |= (1 << 0);      // Enable counter
  TIMX->CNT  = 0;             // Reset counter to 0
}

void delay_ms(TIM_TypeDef* TIMX, uint32_t ms) {
  volatile uint32_t duration = ms * 80000 / (DELAY_PSC + 1);  // convert to ARR val given input clk+psc
  
  TIMX->EGR |=  (1 << 0);       // reset main counter via event flag
  TIMX->ARR  = duration;
  TIMX->SR  &= ~(1 << 0);       // clear update interrupt flag
  while((TIMX->SR & 1) == 0){};
}
