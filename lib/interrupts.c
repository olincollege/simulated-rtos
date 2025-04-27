#include "interrupts.h"

#include <libopencm3/stm32/timer.h>
#include <stdio.h>
#include <stdlib.h>

#include "scheduler.h"
#include "task.h"

extern Queue* global_queue;
extern QueueNode short_task_nodes[MAX_SHORT_TASK];
extern TaskControlBlock short_task_tcbs[MAX_SHORT_TASK];

volatile bool preempt_requested = false;

void tim2_isr(void) {
  if (timer_get_flag(TIM2, TIM_SR_CC1IF)) {
    /* Clear compare interrupt flag. */
    timer_clear_flag(TIM2, TIM_SR_CC1IF);

    printf("Tim2 running. Time: %lu\n", timer_get_counter(TIM2));

    preempt_requested = true;
  }
}

void tim3_isr(void) {
  /* if we get the interrupt flag*/
  if (timer_get_flag(TIM3, TIM_SR_UIF)) {
    /* Clear flag. */
    timer_clear_flag(TIM3, TIM_SR_UIF);

    // Getting time from TIM2 for consistency, but TIM3 and TIM2 are set to
    // the same frequency and should have the same time value
    printf("Tim3 running. Time: %lu\n", timer_get_counter(TIM2));

    /* enqueue the short task*/
    // if no available nodes, meaning that all the prelocated nodes are already
    // in queue, dont queue, else loop through all nodes to find an available
    // node, change to occupied, and queue that node
    for (int i = 0; i < MAX_SHORT_TASK; i++) {
      printf("is available: %d\n", short_task_tcbs[i].is_available);
      if (short_task_tcbs[i].is_available == 1) {
        short_task_tcbs[i].is_available = 0;
        enqueue(global_queue, &short_task_nodes[i]);
        printf("SHORT TASK QUEUED. Time: %lu\n", timer_get_counter(TIM2));
        break;
      }
    }
  }
}
