#pragma once

#define MAX_TASKS 10

#include <pthread.h>
#include <stdio.h>

#include "queue.h"  // Queue

/*
Run the scheduler function. Runs indefinitely.
*/
void run_scheduler(Queue*);
