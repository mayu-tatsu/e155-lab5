// STM32L432KC_TIM1516.h
// Mayu Tatsumi; mtatsumi@g.hmc.edu
// 2025-9-30

// Header file for onboard timers 15 and 16.
// Contains functions to initialize and delay any other
// updates in units of milliseconds, effectively
// lengthening any notes already being played by the 
// speaker.

#ifndef STM32L4_TIM1516_H
#define STM32L4_TIM1516_H

#include <stdint.h>
#include <stm32l432xx.h>

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define __IO volatile

#define DELAY_PSC  (4000)
#define FREQ_PSC   (20)

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void initTIM1516(TIM_TypeDef* TIMX);
void initTIM1516_PWM(TIM_TypeDef* TIMX);
void delay_ms(TIM_TypeDef* TIMX, uint32_t ms);
float delay_ms_polling(TIM_TypeDef* TIMX, uint32_t ms);

#endif
