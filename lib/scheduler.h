#include <stdio.h>
#pragma once
#define MAX_TASKS 10
#include <pthread.h>

/*
Run the scheduler function. Runs indefinitely.
*/
void run_scheduler(Queue*);
