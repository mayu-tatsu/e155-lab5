#include "manual_polling.h"
#include <stdio.h>
#include <stdlib.h>

#include "lib/STM32L432KC_GPIO.h"


volatile int prev_a = 0;
volatile int prev_b = 0;

volatile float velo = 0.0;
volatile float dir = 0.0;

// counts edges since last reset
volatile int polling_edge_count = 0;

#define A_PIN 8
#define B_PIN 10

#define SYSTICK_FREQ 16000

void initSysTick(void) {
  
  SysTick->CTRL = 0;                    // disable
  SysTick->LOAD = SYSTICK_FREQ - 1;     // reload value
  SysTick->VAL  = 0;                    // reset to 0
  SysTick->CTRL = 0x07;                 // enable w/ core clk + interrupts
}

void SysTickHandler(void) {

  int a_curr = digitalRead(A_PIN);
  int b_curr = digitalRead(B_PIN);

  // any edge occurs
  if (a_curr != prev_a) {
    polling_edge_count++;

    if (b_curr == 1) dir = 1.0f;
    else dir = -1.0f;
  }

  prev_a = a_curr;
  prev_b = b_curr;
}
