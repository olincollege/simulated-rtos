#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include "hardware_setup.h"
#include "queue.h"
#include "scheduler.h"
#include "task.h"

/*
Create global queue and task tcbs.
MAX_SHORT_TASK is the number of pre-initialized short task nodes and is defined
in task.h.
*/
// Create global queue pointer so any .c file can access it
Queue* global_queue = NULL;
// Create arrays of short task tcbs and linked list nodes, which will be added
// to the global queue later
TaskControlBlock short_task_tcbs[MAX_SHORT_TASK];
QueueNode short_task_nodes[MAX_SHORT_TASK];

int main(void) {
  clock_setup();
  gpio_setup();
  usart_setup();
  tim2_setup();
  tim3_setup();

  printf("Hello World!\n");

  // Initialize a long task tcb
  TaskControlBlock tcb_long = {long_task, REGULAR_PRIORITY, 0, 0, NULL};
  // Initialize short task tcbs
  for (size_t i = 0; i < MAX_SHORT_TASK; i++) {
    short_task_tcbs[i].func = short_task;
    short_task_tcbs[i].priority = REGULAR_PRIORITY;
    short_task_tcbs[i].is_available = 1;  // available
  }

  // Initialize long task queue node
  QueueNode node_long = {&tcb_long, NULL};

  // Initialize short task queue nodes
  for (size_t i = 0; i < MAX_SHORT_TASK; i++) {
    short_task_nodes[i].tcb = &short_task_tcbs[i];
    short_task_nodes[i].next = NULL;
  }

  // Initialize the global queue with a single long task
  Queue queue = {&node_long, &node_long, 1};
  global_queue = &queue;

  // Scheduler runs forever
  run_scheduler(global_queue);

  return 0;
}
