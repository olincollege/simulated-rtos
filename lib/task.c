#include "task.h"

#include <libopencm3/stm32/timer.h>
#include <stdio.h>

#include "interrupts.h"

/*
simulate tasks here
*/
void long_task(TaskControlBlock* long_task_tcb) {
  if (long_task_tcb->curr_num == 0) {
    printf("Long task started.\n");
  } else {
    printf("Long task resumed.\n");
  }

  while (1) {
    // Cooperatively yield if the timer interrupt has gone off
    if (preempt_requested == 1) {
      return;
    }

    if (long_task_tcb->curr_num == 1000000) {
      // Disable the timer interrupt during task cleanup. No need to turn back
      // on because task is finished
      timer_disable_irq(TIM2, TIM_DIER_CC1IE);
      long_task_tcb->is_active = 0;
      long_task_tcb->curr_num = 0;
      printf("Long task finished.\n");
      return;
    }

    // Temporarily disable the timer interrupt so curr_num doesn't increment
    // without printing the task's progress
    timer_disable_irq(TIM2, TIM_DIER_CC1IE);
    long_task_tcb->curr_num++;
    printf("Long task progress: %d\n", long_task_tcb->curr_num);
    timer_enable_irq(TIM2, TIM_DIER_CC1IE);
  }
}

void short_task(TaskControlBlock* tcb) {
  printf("Short task started\n");
  printf("Short task completed\n");
}

// void task_3(void) { printf("task_3 is running\n"); }
