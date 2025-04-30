#pragma once

#include <pthread.h>
#include <stdio.h>

#include "queue.h"  // Queue

// With the timer running at 5kHz, this many ticks equals a 2 ms delay
extern const int TWO_MILLISECONDS;

/**
 * @brief Run the scheduler function.
 *
 * The scheduler runs indefinitely. It pops a task from the task queue,
 * then calls the function. It also schedules a TIM2 interrupt so that
 * any task it calls can only run for a maximum of 2 ms = 10 TIM2 timer
 * ticks. After a task finishes, it is requeued if it is a long_task.
 *
 * @param queue (Queue*) The task queue (already populated).
 */
void run_scheduler(Queue*);
