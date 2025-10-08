// interrupts.c
// Mayu Tatsumi; mtatsumi@g.hmc.edu
// 2025-10-8

// Implements functions for initializing and handling external
// interrupts. They are triggered by falling or rising edges
// from the encoder signals, and record timestamps accordingly.
// Also, implemented logic to update velocity and direction
// based on those timestamps.

#include "interrupts.h"
#include <stdio.h>
#include <stdlib.h>

#include "lib/STM32L432KC_GPIO.h"

#define A_PIN 8
#define B_PIN 10

volatile int a_val = 0;
volatile int b_val = 0;

// counts edges since last reset
volatile int edge_count = 0;

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
  
        a_val = digitalRead(A_PIN);
        b_val = digitalRead(B_PIN);

        if (a_val != b_val) {
          edge_count++;
        } else {
          edge_count--;
        }
    }
}

void EXTI15_10_IRQHandler(void) {

    // checks if exti line 10 triggered (out of 10-15)
    if (EXTI->PR1 & (1 << B_PIN)) {
        EXTI->PR1 |= (1 << B_PIN);      // clear interrupt flag

        a_val = digitalRead(A_PIN);
        b_val = digitalRead(B_PIN);

        if (a_val == b_val) {
          edge_count++;
        } else {
          edge_count--;
        }
    }
}

float update_velocity(void) {
    // 1 Hz
    float update_freq = 1.0 / (1000.0 / 1000.0);

    // 1/408: converts cycles/s into rev/s, using 408 pulses per rotation
    // div by 4: each revolution has 4 edges
    float vel = edge_count * update_freq * (1.0 / 408.0) / 4.0;
    edge_count = 0;      // reset

    return vel;
}
