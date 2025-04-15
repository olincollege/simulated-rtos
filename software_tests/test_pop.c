#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stddef.h>

#include "../lib/queue.h"
#include "../lib/task.h"

// NOLINTBEGIN(*-magic-numbers)

// Define a task 4 (tasks 1-3 are defined in task.h)
void task_4(void) {}

TaskControlBlock tcb_1 = {task_1, 0, 0};
TaskControlBlock tcb_2 = {task_2, 0, 0};
TaskControlBlock tcb_3 = {task_3, 0, 0};
TaskControlBlock tcb_4 = {task_4, 0, 0};

QueueNode node_4 = {&tcb_4, NULL};
QueueNode node_3 = {&tcb_3, &node_4};
QueueNode node_2 = {&tcb_2, &node_3};
QueueNode node_1 = {&tcb_1, &node_2};

Queue queue = {&node_1, &node_4, 4};

// Test that pop returns the first item in a queue and leaves other items
Test(pop, multi_item_list) {
  QueueNode* node = pop(&queue);
  // Check that pop returns the correct node
  cr_expect(node->tcb->func == task_1);
  // Check that the correct nodes are left in the queue
  cr_expect(queue.front->tcb->func == task_2);
  cr_expect(queue.front->next->tcb->func == task_3);
  cr_expect(queue.last->tcb->func == task_4);
  // Check that the length is three afterwards
  cr_expect(queue.length == (size_t)3);
}

// Test that after popping a 1 item queue the queue is empty
// Test that popping an empty queue returns NULL

// NOLINTEND(*-magic-numbers)
