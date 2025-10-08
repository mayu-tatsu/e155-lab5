// main.c
// Mayu Tatsumi; mtatsumi@g.hmc.edu
// 2025-10-7

// Configures, enables, and instantiates all timers, pins, and
// interrupts necessary. Main loop updates velocity and direction at 1 Hz.

#include <stdio.h>
#include <stm32l432xx.h>

#include "lib/STM32L432KC_FLASH.h"
#include "lib/STM32L432KC_GPIO.h"
#include "lib/STM32L432KC_RCC.h"
#include "lib/STM32L432KC_TIM1516.h"
#include "lib/print_f.h"

#include "interrupts.h"

#define A_PIN 8
#define B_PIN 10

#define MANUAL_POLLING 0

int main(void) {

    configureFlash();
    configureClock();

    enableClkGPIO();
    RCC->APB2ENR |= (1 << 0);     // enable syscfg (for exti)

    pinMode(A_PIN, GPIO_INPUT);
    pinMode(B_PIN, GPIO_INPUT);

    enablePullUp(A_PIN);
    enablePullUp(B_PIN);

    initTIM1516(TIM16);           // delay timer for main loop

    if (!MANUAL_POLLING) {

      __enable_irq();
      initInterrupt(A_PIN);
      initInterrupt(B_PIN);

      volatile float velocity = 0.0f;

      while (1) {
          velocity = update_velocity();

          printf("velocity: %f rev/s\n", velocity);

          // 1 Hz loop delay
          delay_ms(TIM16, 1000);
      }

    } else {
      
      volatile float velocity = 0.0f;
      while (1) {
        velocity = delay_ms_polling(TIM16, 500);
        printf("velocity (manual polling): %f rev/s\n", velocity);
        
      }
    }
}
