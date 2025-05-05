#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stddef.h>  // size_t

#include "../lib/queue.h"
#include "../lib/task.h"

// NOLINTBEGIN(*-magic-numbers)

// Define test tasks
// They are cast to void to supress unused parameter compiler warnings
void task_1(TaskControlBlock* tcb) { (void)tcb; }
void task_2(TaskControlBlock* tcb) { (void)tcb; }
void task_3(TaskControlBlock* tcb) { (void)tcb; }
void task_4(TaskControlBlock* tcb) { (void)tcb; }

// Set up tcb blocks for test queues
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
TaskControlBlock tcb_1 = {task_1, 0, 0, 0, NULL, NULL};
TaskControlBlock tcb_2 = {task_2, 0, 0, 0, NULL, NULL};
TaskControlBlock tcb_3 = {task_3, 0, 0, 0, NULL, NULL};
TaskControlBlock tcb_4 = {task_4, 0, 0, 0, NULL, NULL};
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

/*
Tests for enqueue
*/

// Test that enqueue queues a node on an existing queue
Test(enqueue, add_to_empty) {
  QueueNode node_1 = {&tcb_1, NULL};
  Queue queue = {NULL, NULL, 0};
  enqueue(&queue, &node_1);
  // check that the first task is enqueued properly
  cr_expect(queue.front->tcb->func == task_1);
  // check that the last task is enqueued properly
  cr_expect(queue.last->tcb->func == task_1);
  // check that the length is 1 after enqueue
  cr_expect(queue.length == (size_t)1);
}

/*
Test for enqueuing a non empty list
*/
// Test that enqueue queues a node on an existing queue
Test(enqueue, add_to_normal) {
  QueueNode node_1 = {&tcb_1, NULL};
  QueueNode node_2 = {&tcb_2, NULL};

  Queue queue = {&node_1, &node_1, 1};

  enqueue(&queue, &node_2);
  // check that the task is enqueued properly
  cr_expect(queue.last->tcb->func == task_2);
  // check that the first node is linked correctly to the second node
  cr_expect(queue.front->next->tcb->func == task_2);
  // check that the length is 2 after enqueue
  cr_expect(queue.length == (size_t)2);
}

/*
Tests for pop
*/

// Test that pop returns the first item in a queue and leaves other items
Test(pop, multi_item_list) {
  // Create a test queue
  QueueNode node_4 = {&tcb_4, NULL};
  QueueNode node_3 = {&tcb_3, &node_4};
  QueueNode node_2 = {&tcb_2, &node_3};
  QueueNode node_1 = {&tcb_1, &node_2};

  Queue queue = {&node_1, &node_4, 4};

  QueueNode* node = pop(&queue);
  // Check that pop returns the correct node
  cr_expect(node->tcb->func == task_1);
  // Check that the correct nodes are left in the queue
  cr_expect(queue.front->tcb->func == task_2);
  cr_expect(queue.front->next->tcb->func == task_3);
  cr_expect(queue.front->next->next->tcb->func == task_4);
  cr_expect(queue.last->tcb->func == task_4);
  cr_expect(queue.last->next == NULL);
  // Check that the length is three afterwards
  cr_expect(queue.length == (size_t)3);
}

// Test that after popping a 1 item queue the queue is empty
Test(pop, single_item_list) {
  // Create a test queue
  QueueNode node_1 = {&tcb_1, NULL};

  Queue queue = {&node_1, &node_1, 1};

  QueueNode* node = pop(&queue);
  // Check that pop returns the correct node
  cr_expect(node->tcb->func == task_1);
  // Check that all pointers to nodes are NULL
  cr_expect(queue.front == NULL);
  cr_expect(queue.last == NULL);
  // Check that the length is 0 afterwards
  cr_expect(queue.length == (size_t)0);
}

// Test that popping an empty queue returns NULL
Test(pop, zero_item_list) {
  // Create a test queue
  Queue queue = {NULL, NULL, 0};

  QueueNode* node = pop(&queue);
  // Check that pop returns NULL
  cr_expect(node == NULL);
  // Check that all pointers to nodes are NULL
  cr_expect(queue.front == NULL);
  cr_expect(queue.last == NULL);
  // Check that the length is 0 afterwards
  cr_expect(queue.length == (size_t)0);
}

// NOLINTEND(*-magic-numbers)
