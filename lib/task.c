#include "task.h"

#include <stdio.h>

/*
simulate tasks here
*/
void long_task(TaskControlBlock* long_task_tcb) {
  for (; long_task_tcb->curr_num < 1000000;) {
    if (long_task_tcb->curr_num % 1000000 == 0) {
      __disable_irq();
      long_task_tcb->is_active = 0;
      printf("long task finished.\n");
      __enable_irq();
    }
    __disable_irq();  // libopencm3 or CMSIS function to disable interrupts
    long_task_tcb->curr_num++;
    printf("long task progress, %d\n", long_task_tcb->curr_num);
    __enable_irq();  // libopencm3 or CMSIS function to disable interrupts
  }
}
void short_task(void) { printf("short task is running\n"); }
void task_3(void) { printf("task_3 is running\n"); }
