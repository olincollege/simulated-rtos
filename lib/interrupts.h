// saves the curr state of the task, queue the task to the end of the queue, run
// scheduler
#pragma once
#include <stdbool.h>  //bool

#include "queue.h"

extern volatile bool preempt_requested;

void tim2_isr(void);

/**
 * @brief Enqueue a short_task every
 */
void tim3_isr(void);
