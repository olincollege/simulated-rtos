#pragma once

#include <stddef.h>  // size_t

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

// TODO remove stuff about warning priority if not using
/**
 * Enqueue a new task onto the queue.
 *
 * This function inserts a new task at the appropriate position in the queue
 * based on its priority. If the queue is empty, the new task becomes both the
 * front and the last element of the queue. If the task has a WARNING_PRIORITY,
 * it is inserted at the front of the queue, bypassing the regular queue order.
 * Otherwise, the task is added to the end of the queue.
 *
 * @param queue (Queue*): A pointer to the queue in which the new task should be
 *                        enqueued.
 * @param new_task (QueueNode*): A pointer to the task (QueueNode) to be
 *                               added to the queue.
 */
void enqueue(Queue* queue, QueueNode* new_task);

/**
 * Remove and return the front task from the queue.
 *
 * This function removes the task at the front of the queue and returns a
 * pointer to it. If the queue is empty, it returns NULL. After the task is
 * removed, the front of the queue is updated. If the queue becomes empty after
 * the operation, the last pointer is also set to NULL.
 *
 * @param queue (Queue*): A pointer to the queue from which the task should be
 *                   removed.
 *
 * @return QueueNode*: A pointer to the task (QueueNode) that was removed
 */
QueueNode* pop(Queue* queue);
