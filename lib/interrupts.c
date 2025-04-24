#include "interrupts.h"

#include <libopencm3/stm32/timer.h>
#include <stdio.h>
#include <stdlib.h>

#include "scheduler.h"
#include "task.h"

volatile bool preempt_requested = false;

void tim2_isr(void) {
  if (timer_get_flag(TIM2, TIM_SR_CC1IF)) {
    /* Clear compare interrupt flag. */
    timer_clear_flag(TIM2, TIM_SR_CC1IF);

    printf("Interrupt handler running\n");

    preempt_requested = true;
  }
}
