#include "interrupts.h"

#include <stdio.h>
#include <stdlib.h>

#include "scheduler.h"
#include "task.h"
#pragma once

void interrupt(TaskControlBlock* interrupted_task, Queue* queue) {
  QueueNode* task_node;
  task_node->tcb = interrupted_task;

  enqueue(queue, task_node);
  run_scheduler(queue);
}

// This is the example isr function for SOS led blink
void tim2_isr(void) {
  if (timer_get_flag(TIM2, TIM_SR_CC1IF)) {
    /* Clear compare interrupt flag. */
    timer_clear_flag(TIM2, TIM_SR_CC1IF);

    /*
     * Get current timer value to calculate next
     * compare register value.
     */
    uint16_t compare_time = timer_get_counter(TIM2);

    /* Calculate and set the next compare value. */
    uint16_t frequency = frequency_sequence[frequency_sel++];
    uint16_t new_time = compare_time + frequency;

    timer_set_oc_value(TIM2, TIM_OC1, new_time);
    if (frequency_sel == ARRAY_LEN(frequency_sequence)) {
      frequency_sel = 0;
    }

    printf("LED toggled!\n");
    /* Toggle LED to indicate compare event. */
    gpio_toggle(LED1_PORT, LED1_PIN);
  }
}
