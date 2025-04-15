#pragma once

#include "task.h"

typedef struct QueueNode {
  struct TaskControlBlock* tcb;
  struct QueueNode* next;
} QueueNode;

typedef struct {
  QueueNode* front;  // pointer to the first element in queue
  QueueNode* last;   // pointer to the last task in queue
  size_t length;     // length of queue
} Queue;

void enqueue(Queue* queue, QueueNode* new_task);

QueueNode* pop(Queue* queue);
