/**
* @file encoder_config.c
* @brief Configuration for TIM2 in Encoder Interface Mode on STM32L432KC.
*
* This configuration uses PA0 (TIM2_CH1) for Channel A and PA1 (TIM2_CH2) for Channel B.
* The timer is configured to count up/down based on the phase relationship between A and B.
* An interrupt is enabled on Update Event (overflow/underflow) to track total rotations.
*/


#include "stm32l432xx.h"


// Define the maximum value of the 16-bit counter (65535)
#define TIMER_PERIOD_16BIT 0xFFFF


/**
* @brief Initializes the GPIO pins and the TIM2 peripheral for quadrature encoder interface.
*/
void Encoder_Init(void) {
   // 1. Enable Clocks for GPIOA and TIM2
   RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
   RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;


   // 2. Configure PA0 and PA1 as Alternate Function Mode (AF)
   // Mode[1:0] = 10 (AF mode)
   GPIOA->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1);
   GPIOA->MODER |= (GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1);


   // 3. Configure PA0 and PA1 Alternate Function to AF1 (TIM2)
   // AFRL0 and AFRL1 fields in AFRL register set to 0001 (AF1)
   GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL0 | GPIO_AFRL_AFSEL1);
   GPIOA->AFR[0] |= (1 << GPIO_AFRL_AFSEL0_Pos) | (1 << GPIO_AFRL_AFSEL1_Pos);


   // 4. Configure TIM2 Slave Mode Controller Register (TIM2->SMCR)
   // SMS = 011 (Encoder mode 3 - counts on both TI1 and TI2)
   TIM2->SMCR &= ~TIM_SMCR_SMS;
   TIM2->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1; // 011


   // 5. Configure Capture/Compare Mode Register (TIM2->CCMR1)
   // CC1S = 01 (TI1 input selected)
   // CC2S = 01 (TI2 input selected)
   TIM2->CCMR1 &= ~(TIM_CCMR1_CC1S | TIM_CCMR1_CC2S);
   TIM2->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0); // 01


   // 6. Set Auto-Reload Register (ARR) for 16-bit rollover
   TIM2->ARR = TIMER_PERIOD_16BIT;


   // 7. Enable Timer Update Interrupt (UIE bit)
   TIM2->DIER |= TIM_DIER_UIE;


   // 8. Enable TIM2 Interrupt in NVIC
   NVIC_EnableIRQ(TIM2_IRQn);


   // 9. Enable the Counter (CEN bit)
   TIM2->CR1 |= TIM_CR1_CEN;
}
