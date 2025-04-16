#pragma once

typedef void (*TaskFunc)(void);
// a pointer to a function that takes in nothing and returns void
typedef struct TaskControlBlock {
  TaskFunc func;
  int priority;
  int is_active;
  // task ID
} TaskControlBlock;
const int REGULAR_PRIORITY = 3;
const int WARNING_PRIORITY = 1;
void task_1(void);
void task_2(void);
void task_3(void);
