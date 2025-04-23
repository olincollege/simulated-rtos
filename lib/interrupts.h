//saves the curr state of the task, queue the task to the end of the queue, run scheduler
#pragma once
#include "queue.h"

void interrupt(TaskControlBlock* interrupted_task, Queue* queue);
