#pragma once
#include <stdbool.h>

struct TaskControlBlock;

// Set the maximum number of short tasks, whose nodes will be preallocated
const size_t MAX_SHORT_TASK = 15;

// a pointer to a function that takes in nothing and returns void
typedef void (*TaskFunc)(struct TaskControlBlock*);

typedef enum { WARNING_PRIORITY = 1, REGULAR_PRIORITY = 3 } TaskPriority;

typedef struct TaskControlBlock {
  TaskFunc func;
  TaskPriority priority;
  int is_available;
  int curr_num;
  struct QueueNode* my_node;
  // task ID
} TaskControlBlock;

void long_task(TaskControlBlock* long_task_tcb);
void short_task(TaskControlBlock* short_task_tcb);
