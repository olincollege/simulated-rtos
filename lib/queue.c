#include "queue.h"

#include <stdio.h>

#include "task.h"

void enqueue(Queue* q, QueueNode* new_task) {
  if (q->length == 0) {
    q->front = new_task;
    q->last = new_task;
    q->length = 1;
    return;
  }
  q->last->next = new_task;
};

QueueNode* pop(Queue* q) {
  if (q->length == 0) {
    perror("queue is empty");
    return;
  }
  q->length -= 1;
  return q->front;
};
