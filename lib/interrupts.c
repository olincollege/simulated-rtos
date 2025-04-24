#include "interrupts.h"

#include <stdio.h>
#include <stdlib.h>

#include "scheduler.h"
#include "task.h"
#pragma once

void tim2_isr(void) {
  if (timer_get_flag(TIM2, TIM_SR_CC1IF)) {
    /* Clear compare interrupt flag. */
    timer_clear_flag(TIM2, TIM_SR_CC1IF);

    preempt_requested = true;
  }
}
