
// import everything
//  Define tasks 1, 2, and 3

//  main()
//  Make task queue
//  Add tasks 1,2,3 to task queue
//  Run the scheduler

#include <stdio.h>

#include "../scheduler/scheduler.h"
#include "task.h"
int main(void) {
  // create 3 tasks and run scheduler
  create_task(task1, 1);
  create_task(task2, 2);
  create_task(task3, 0);  // the second args is the priority

  run_scheduler();
  return 0;
}
