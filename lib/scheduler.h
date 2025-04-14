

#include <stdio.h>
#pragma once
#define MAX_TASKS 10
#include <pthread.h>

typedef void (*Task_func)();
// a pointer to a function that takes in nothing and returns void
typedef struct {
  Task_func func;
  int priority;
  int is_active;
  // task ID
  //  Stack pointer so we can interrupt and resume task
  //

} TaskControlBlock;
