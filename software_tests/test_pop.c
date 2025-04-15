#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../lib/queue.h"
#include "../lib/task.h"

// NOLINTBEGIN(*-magic-numbers)

TaskControlBlock tcb_1 = {task_1, 0, 0};
TaskControlBlock tcb_2 = {task_2, 0, 0};
TaskControlBlock tcb_3 = {task_3, 0, 0};

QueueNode node_3 = {&tcb_3, NULL};
QueueNode node_2 = {&tcb_2, &task_3};
QueueNode node_1 = {&tcb_1, &task_2};

Queue queue = {&node_1, &node_3, 3};

// Test that pop returns the first item in a queue and leaves other items
Test(pop, multi_item_list) {
  QueueNode* node = pop(&queue);
  cr_expect(node->tcb->func == task_1);
}

// Test that after popping a 1 item queue the queue is empty
// Test that popping an empty queue returns NULL

// NOLINTEND(*-magic-numbers)
