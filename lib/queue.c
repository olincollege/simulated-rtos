#include "queue.h"

#include <stdio.h>
#include <stdlib.h>

void enqueue(Queue* queue, QueueNode* new_task) {
  if (queue->length == 0) {
    queue->front = new_task;
    queue->last = new_task;
    queue->length = 1;
    return;
  }
  if (new_task->tcb->priority == WARNING_PRIORITY) {
    QueueNode* old_task = queue->front;
    queue->front = new_task;        // skips the queue bc it's urgent
    queue->front->next = old_task;  // connect back to queue
    queue->length += 1;

  } else  // else it's just regular priority
  {
    queue->last->next = new_task;
    queue->last = new_task;
    queue->length += 1;
  }
}

QueueNode* pop(Queue* queue) {
  if (queue->length == 0) {
    return NULL;
  }
  QueueNode* pop_node = queue->front;
  queue->length -= 1;
  queue->front = queue->front->next;
  if (queue->length == 0) {
    queue->last = NULL;
  }
  return pop_node;
}
