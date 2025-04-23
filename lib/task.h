#pragma once

typedef void (*TaskFunc)(void);
// a pointer to a function that takes in nothing and returns void
typedef enum { WARNING_PRIORITY = 1, REGULAR_PRIORITY = 3 } TaskPriority;

typedef struct TaskControlBlock {
  TaskFunc func;
  TaskPriority priority;
  int is_active;
  int curr_num;
  // task ID
} TaskControlBlock;
void task_1(void);
void task_2(void);
void task_3(void);
