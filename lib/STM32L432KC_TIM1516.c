// STM32L432KC_TIM1516.c
// Mayu Tatsumi; mtatsumi@g.hmc.edu
// 2025-9-30

// Defines all functions required for playing the speaker,
// building off the _TIM1516 header file. Has functions for
// the instantiation for both pins 15 and 16. 15 functions
// just as a timer, and 16 as a PWM. delay_ms creates a
// delay via while loop with nothing in it, taking timer 15.

#include "STM32L432KC_TIM1516.h"
#include "STM32L432KC_GPIO.h"

#define A_PIN 8
#define B_PIN 10

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

float delay_ms_polling(TIM_TypeDef* TIMX, uint32_t ms) {
  volatile uint32_t duration = ms * 80000 / (DELAY_PSC + 1);  // convert to ARR val given input clk+psc
  
  TIMX->EGR |=  (1 << 0);       // reset main counter via event flag
  TIMX->ARR  = duration;
  TIMX->SR  &= ~(1 << 0);       // clear update interrupt flag'


  volatile float a_val = 0;
  volatile float b_val = 0;
  volatile float prev_a_val = 0;
  volatile float prev_b_val = 0;

  volatile int count = 0;

  volatile float vel = 0.0;
  volatile float dir = 0.0;     // 1.0 for CW, -1.0 for CCW

  while((TIMX->SR & 1) == 0){
  
    prev_a_val = a_val; 
    prev_b_val = b_val; 
    
    a_val = digitalRead(A_PIN);
    b_val = digitalRead(B_PIN);

    if (a_val != prev_a_val || b_val != prev_b_val) {
      if (a_val != b_val && prev_a_val < a_val)         // rising edge for A, first -> CW
        dir = 1.0;
      else if (a_val != b_val && prev_b_val < b_val)    // rising edge for B, first -> CCW
        dir = -1.0;
      count++;
    }
  }

  float update_freq = 1.0 / (500 / 1000.0);
  vel = count * update_freq * (1.0 / 408.0) / 2.0;
  count = 0;

  return vel * dir;
}