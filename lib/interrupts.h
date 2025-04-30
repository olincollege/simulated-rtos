// saves the curr state of the task, queue the task to the end of the queue, run
// scheduler
#pragma once
#include <stdbool.h>  //bool

#include "queue.h"

// Flag used by tim2_isr to mark when a task function should yield. Task
// functions check for this often.
extern volatile bool preempt_requested;

/**
 * @brief Stop a task after it has run for 10 ticks.
 *
 * This interrupt is set by the scheduler and canceled if a task finishes before
 * 10 ticks are up. Clears the interrupt flag, prints a log message, and sets
 * the preempt_requested flag to true.
 */
void tim2_isr(void);

/**
 * @brief Enqueue a short_task every 3 ticks.
 *
 * Clear the TIM3 interrupt flag, print a log message, and add a short task node
 * to the global queue.
 */
void tim3_isr(void);
