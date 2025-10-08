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

int main(void) {

    configureFlash();
    configureClock();

    enableClkGPIO();
    RCC->APB2ENR |= (1 << 0); // enable syscfg (for exti)

    pinMode(A_PIN, GPIO_INPUT);
    pinMode(B_PIN, GPIO_INPUT);

    enablePullUp(A_PIN);
    enablePullUp(B_PIN);

    __enable_irq();
    initInterrupt(A_PIN);
    initInterrupt(B_PIN);

    initTIM1516(TIM15);         // ref timer to measure edge times
    initTIM1516(TIM16);         // delay timer for main loop

    volatile float velocity = 0.0f;
    volatile float direction = -1.0f;
    int loop_delay = 500; // ms

    while (1) {
        velocity = update_velocity(loop_delay);
        direction = update_direction();

        printf("%f rev/s\n", velocity * direction);

        // 1 Hz loop delay
        delay_ms(TIM16, loop_delay);
    }
}
