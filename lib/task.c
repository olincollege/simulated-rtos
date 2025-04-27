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
      printf("Long task paused. Time: %lu", timer_get_counter(TIM2));
      preempt_requested = false;
      return;
    }

    if (long_task_tcb->curr_num == 1000) {
      // Task finished
      long_task_tcb->curr_num = 0;  // reset curr_num
      // The long task is always requeued, so no need to reset availability
      printf("Long task finished. Time: %lu\n", timer_get_counter(TIM2));
      return;
    }

    long_task_tcb->curr_num++;
    printf("Long task progress: %d\n", long_task_tcb->curr_num);
  }
}

void short_task(TaskControlBlock* tcb) {
  if (tcb->curr_num == 0) {
    printf("Short task started. Time: %lu\n", timer_get_counter(TIM2));
  } else {
    printf("Short task resumed. Time: %lu\n", timer_get_counter(TIM2));
  }

  while (1) {
    // Cooperatively yield if the timer interrupt has gone off
    if (preempt_requested == true) {
      printf("Short task paused. Time: %lu", timer_get_counter(TIM2));
      preempt_requested = false;
      return;
    }

    if (tcb->curr_num == 100) {
      // Task finished
      tcb->is_available = 1;
      tcb->curr_num = 0;
      printf("Short task finished. Time: %lu\n", timer_get_counter(TIM2));
      return;
    }

    tcb->curr_num++;
    printf("Short task progress: %d\n", tcb->curr_num);
  }
}
