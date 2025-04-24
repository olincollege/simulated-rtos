// saves the curr state of the task, queue the task to the end of the queue, run
// scheduler
#pragma once
#include <stdbool.h>  //bool

#include "queue.h"

volatile bool preempt_requested = false;

void tim2_isr(void);
