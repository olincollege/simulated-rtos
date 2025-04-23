
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
