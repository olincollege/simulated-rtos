#include "scheduler.h"

#include <libopencm3/stm32/timer.h>

#include "interrupts.h"  //preempt_requested
#include "queue.h"
#include "task.h"

const int TWO_MILLISECONDS = 10;

void run_scheduler(Queue* queue) {
  while (1) {
    // Schedule an interrupt after 2 ms of running the task
    /* Set the initual output compare value for OC1. */
    timer_set_oc_value(TIM2, TIM_OC1, TWO_MILLISECONDS);
    /* Counter enable. */
    timer_enable_counter(TIM2);
    /* Enable Channel 1 compare interrupt.*/
    timer_enable_irq(TIM2, TIM_DIER_CC1IE);

    // pop task node from queue
    QueueNode* node = pop(queue);
    // if node is long task, then switch to active??

    // run task
    node->tcb->func(node->tcb);

    enqueue(queue, node);
  }
}
