#include "task.h"

#include <libopencm3/stm32/timer.h>
#include <stdio.h>

#include "interrupts.h"

/*
simulate tasks here
*/
void long_task(TaskControlBlock* long_task_tcb) {
  if (long_task_tcb->curr_num == 0) {
    printf("Long task started. Time: %lu\n", timer_get_counter(TIM2));
  } else {
    printf("Long task resumed. Time: %lu\n", timer_get_counter(TIM2));
  }

  while (1) {
    // Cooperatively yield if the timer interrupt has gone off
    if (preempt_requested == true) {
      preempt_requested = false;
      return;
    }

    if (long_task_tcb->curr_num == 1000) {
      long_task_tcb->is_active = 0;
      long_task_tcb->curr_num = 0;
      printf("Long task finished. Time: %lu\n", timer_get_counter(TIM2));
      return;
    }

    long_task_tcb->curr_num++;
    printf("Long task progress: %d\n", long_task_tcb->curr_num);
  }
}

void short_task(TaskControlBlock* tcb) {
  printf("Short task started. Time: %lu\n", timer_get_counter(TIM2));
  printf("Short task completed. Time: %lu\n", timer_get_counter(TIM2));
}

// void task_3(void) { printf("task_3 is running\n"); }
