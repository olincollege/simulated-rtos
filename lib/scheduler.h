#pragma once

#include <pthread.h>
#include <stdio.h>

#include "queue.h"  // Queue

// With the timer running at 5kHz, this many ticks equals a 2 ms delay
const int TWO_MILLISECONDS = 10;

/*
Run the scheduler function. Runs indefinitely.
*/
void run_scheduler(Queue*);
