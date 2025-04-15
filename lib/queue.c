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
  queue->last->next = new_task;
  queue->last = new_task;
  queue->length += 1;
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
