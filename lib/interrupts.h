// saves the curr state of the task, queue the task to the end of the queue, run
// scheduler
#pragma once
#include <stdbool.h>  //bool

#include "queue.h"

extern volatile bool preempt_requested;

void tim2_isr(void);

/*
enqueue short task to queue when tim3 timer is up
*/
void tim3_isr(void);
