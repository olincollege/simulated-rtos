/*This is a copy of task.c with the TIM2 print statements removed. Our CMake
 * setup can't deal with anything involving libopencm3 so we're recreating it
 * here for testing our queue functionality.*/

#include "task.h"

// #include <libopencm3/stm32/timer.h>
#include <stdio.h>

#include "interrupts.h"

void long_task(TaskControlBlock* long_task_tcb) {
  // Print if long task is starting or resuming
  //   printf("Long task %s. Time: %lu\n",
  //          long_task_tcb->curr_num == 0 ? "started" : "resumed",
  //          timer_get_counter(TIM2));

  while (1) {
    // Cooperatively yield if the timer interrupt has gone off
    if (preempt_requested == true) {
      //   printf("Long task paused. Time: %lu\n", timer_get_counter(TIM2));
      preempt_requested = false;
      return;
    }

    if (long_task_tcb->curr_num == LONG_TASK_COUNT_LENGTH) {
      // Task finished
      long_task_tcb->curr_num = 0;  // reset curr_num
      // The long task is always requeued, so no need to reset availability
      //   printf("Long task finished. Time: %lu\n", timer_get_counter(TIM2));
      return;
    }

    long_task_tcb->curr_num++;
    printf("Long task progress: %d\n", long_task_tcb->curr_num);
  }
}

void short_task(TaskControlBlock* tcb) {
  // Print if short task is starting or resuming
  //   printf("Short task %s. Time: %lu\n",
  //          tcb->curr_num == 0 ? "started" : "resumed",
  //          timer_get_counter(TIM2));

  while (1) {
    // Cooperatively yield if the timer interrupt has gone off
    if (preempt_requested == true) {
      //   printf("Short task paused. Time: %lu\n", timer_get_counter(TIM2));
      preempt_requested = false;
      return;
    }

    if (tcb->curr_num == SHORT_TASK_COUNT_LENGTH) {
      // Task finished
      tcb->is_available = 1;
      tcb->curr_num = 0;
      //   printf("Short task finished. Time: %lu\n", timer_get_counter(TIM2));
      return;
    }

    tcb->curr_num++;
    printf("Short task progress: %d\n", tcb->curr_num);
  }
}

void dependent_task(TaskControlBlock* tcb) {
  // Print if dependent task is starting or resuming
  //   printf("Dependent task %s. Time: %lu\n",
  //          tcb->curr_num == 0 ? "started" : "resumed",
  //          timer_get_counter(TIM2));

  while (1) {
    // Cooperatively yield if the timer interrupt has gone off
    if (preempt_requested == true) {
      //   printf("Dependent task paused. Time: %lu\n",
      //   timer_get_counter(TIM2));
      preempt_requested = false;
      return;
    }

    if (tcb->curr_num == SHORT_TASK_COUNT_LENGTH) {
      // Task finished
      tcb->is_available = 1;
      tcb->curr_num = 0;
      //   printf("Dependent task finished. Time: %lu\n",
      //   timer_get_counter(TIM2));
      return;
    }

    tcb->curr_num++;
    printf("Dependent task progress: %d\n", tcb->curr_num);
  }
}
