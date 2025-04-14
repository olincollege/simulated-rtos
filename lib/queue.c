#include "queue.h"

#include <stdio.h>

#include "task.h"

int queue_size = 0;
// return queue length
// pop the highest priority
// insert task to queue
//

/* Task Queue (Priority queue, add task and sort, remove no need to sort) */
static struct TaskControlBlock gs_task_queue[MAX_NUMBER_OF_TASKS];
/* Task queue length */
static uint32_t task_queue_len;
/* Id for the task (must be unique for all the active tasks) */
static uint32_t next_task_id;

// initialize task queue
void init_task_queue() {
  /* Reset the queue index */
  task_queue_len = 0;

  /* Reset the task id */
  next_task_id = 0;
}

int queue_is_empty(void) { return queue_size == 0; }
int insert_task(int task_id, int priority_num) {}
struct TaskControlBlock* pop_task  // returns task ID?
