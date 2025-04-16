#include "scheduler.h"

#include "queue.h"
#include "task.h"

int task_count = 0;

void run_scheduler(Queue* queue) {
  while (1) {
    // pop task node from queue
    QueueNode* node = pop(queue);
    TaskControlBlock* tcb = node->tcb;

    // run task
    tcb->func();

    enqueue(queue, node);
  }
}
