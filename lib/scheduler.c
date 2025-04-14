#include "scheduler.h"

int task_count = 0;

void run_scheduler() {
  while (curr_running_task < MAX_TASKS) {
    // inspect task queue
    for (int i = 0; i < task_count; ++i) {
      if (taskList[i].priority > highestPriority)
        // run

        while (1) {
          /* Loop on the task queue */
          while (i < get_task_queue_length()) {
            /* Get the task id at the ith position */
            g_id = get_task_id(i);

            /* Take action according to the state of the task */
            switch (get_task_state(g_id)) {
              case TASK_IS_CREATED:
                /* Initialize the base pointer for the task function */
                GET_NEXT_RBP(g_rbp);
                /* Get the task function */
                p_task = get_task_function(g_id);
                /* Call the task function */
                p_task();
                /* Check if the task returned through yield or directly */
                if (g_yield_return) {
                  /* Reset the global flag */
                  g_yield_return = false;
                } else {
                  /* Remove the task from the queue */
                  remove_task(g_id);
                }

                /* Reset the value of i to 0 */
                i = 0;
                break;

              case TASK_IS_READY:
                /* Load the task function again */
                load_from_scheduler();
                /* Check if the task returned through yield or directly */
                if (g_yield_return) {
                  /* Reset the global flag */
                  g_yield_return = false;
                } else {
                  /* Remove the task from the queue */
                  remove_task(g_id);
                }

                /* Reset the value of i to 0 */
                i = 0;
                break;

              case TASK_IS_WAITING_FOR_XX:

                /* If the task is waiting then increment to check next task */
                i++;
                break;
            }
          }

          /* Reset the index */
          i = 0;
        }
    }
  }

  //
}
}
