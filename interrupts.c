#include "interrupts.h"
#include <stdio.h>
#include <stdlib.h>

// timestamps
volatile int a_rising = 0;
volatile int b_rising = 0;
volatile int a_falling = 0;
volatile int b_falling = 0;

volatile int a_state = 0;
volatile int b_state = 0;

// counts edges since last reset
volatile int edge_count = 0;

#define A_PIN 8
#define B_PIN 10

void initInterrupt(int pin) {
    SYSCFG->EXTICR[pin / 4] &= ~(0xF << ((pin % 4) * 4));       // clear
    SYSCFG->EXTICR[pin / 4] |=  (0b000 << ((pin % 4) * 4));     // set

    EXTI->IMR1 |= (1 << pin);       // unmask

    EXTI->RTSR1 |= (1 << pin);      // enable rising edge trigger
    EXTI->FTSR1 |= (1 << pin);      // enable falling edge trigger

    // enable exti interrupts in nvic
    if (pin >= 10 && pin <= 15) {                       // exti 10–15
        NVIC->ISER[1] |= (1 << (EXTI15_10_IRQn - 32));  // ISER1 40-32
    } else if (pin >= 5 && pin <= 9) {                  // exti 5–9
        NVIC->ISER[0] |= (1 << EXTI9_5_IRQn);           // ISER0 23
    }
}

void EXTI9_5_IRQHandler(void) {

    // checks if exti line 8 triggered (out of 5-8)
    if (EXTI->PR1 & (1 << A_PIN)) {
        EXTI->PR1 |= (1 << A_PIN);        // clear interrupt flag

        if (a_state == 0) {               // if rising:
            TIM15->EGR |= 1;              // reset
            a_rising = TIM15->CNT;        // record
        } else {                          // if falling:
            a_falling = TIM15->CNT;       // record
        }

        // toggle
        a_state = ~a_state;

        // printf("interrupt triggered\n");
    }
}

void EXTI15_10_IRQHandler(void) {

    // checks if exti line 10 triggered (out of 10-15)
    if (EXTI->PR1 & (1 << B_PIN)) {
        EXTI->PR1 |= (1 << B_PIN);      // clear interrupt flag

        if (b_state == 0) {             // if rising:
            b_rising = TIM15->CNT;      // record
        } else {                        // if falling:
            b_falling = TIM15->CNT;     // record
        }

        // toggle + increment
        b_state = ~b_state;
        edge_count++;

        // if (!(edge_count % 10))
        //   printf("edge count: %d at interrupt\n", edge_count);
    }
}

float update_velocity(int loop_delay) {
    // dividing by 1000: converts ms -> s
    float update_freq = 1.0 / (loop_delay / 1000.0);

    // 1/408: converts cycles/s into rev/s, using 408 pulses per rotation
    // div by 2: each rev has 2 rising edges
    float vel = edge_count * update_freq * (1.0 / 408.0) / 2.0;
    edge_count = 0;      // reset

    return vel;
}

float update_direction(void) {
    // compare most recent rising and falling edges
    // if rising > falling, last edge was rising -> dir = 1
    if (b_rising > b_falling) return 1.0f;
    else return -1.0f;
}
