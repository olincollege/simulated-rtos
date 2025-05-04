#pragma once

#include <stdbool.h>
#include <stddef.h>  // size_t

struct TaskControlBlock;

// Set the maximum number of short tasks, whose nodes will be preallocated
enum { MAX_SHORT_TASK = 15 };

// A pointer to a task function, which takes in a tcb and returns void
typedef void (*TaskFunc)(struct TaskControlBlock*);

// This priority functionality is currently unused but kept in case we want to
// add task priorities in the future.
typedef enum { WARNING_PRIORITY = 1, REGULAR_PRIORITY = 3 } TaskPriority;

typedef struct TaskControlBlock {
  TaskFunc func;          // The task function to run
  TaskPriority priority;  // Currently unused
  int is_available;  // Only used for short tasks, marks if the preallocacted
                     // tcb node is being used already
  int curr_num;      // The current "state" of the running task (0 if it hasn't
                     // started)
  struct QueueNode*
      my_node;  // Pointer to the wrapper queue node the task is inside
  struct TaskControlBlock* next_task;  // For short tasks only -- next task in
                                       // the linked list of preallocated tasks
} TaskControlBlock;

/**
 * @brief Simulate a long task by printing the numbers 1 to 1000.
 *
 * This task can be "preempted" and then "resumed" because it continuously saves
 * the most recent number it has printed. Each time a number is printed, it
 * checks if a preempt has been requested (returning if it has) and saves the
 * number is on in its tcb->curr_num. If it has finished by reaching 1000, it
 * resets curr_num and returns.
 */
void long_task(TaskControlBlock* long_task_tcb);

/**
 * @brief Simulate a short task by printing the numbers 1 to 100.
 *
 * This task can be "preempted" and then "resumed" because it continuously saves
 * the most recent number it has printed. Each time a number is printed, it
 * checks if a preempt has been requested (returning if it has) and saves the
 * number is on in its tcb->curr_num. If it has finished by reaching 100, it
 * resets curr_num, marks its task tcb as available, and returns.
 */
void short_task(TaskControlBlock* short_task_tcb);

/**
 * @brief Simulate a task that is dependent on the short task.
 *
 * This task behaves identically to the short task, printing numbers from 1 to
 * 100 with support for preemption and resumption via tcb->curr_num. However, it
 * is constrained to execute *immediately after* the short task without any
 * other task running in between. This simulates a task that is "bundled" with
 * the short task, where the short task must complete before this dependent task
 * runs, and no tasks can run in between these two tasks.
 */
void dependent_task(TaskControlBlock* dependent_task_tcb);
