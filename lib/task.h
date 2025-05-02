#pragma once

#include <stdbool.h>
#include <stddef.h>  // size_t

struct TaskControlBlock;

// Set the maximum number of short tasks, whose nodes will be preallocated
enum { MAX_SHORT_TASK = 15 };

// A pointer to a task function, which takes in a tcb and returns void
typedef void (*TaskFunc)(struct TaskControlBlock*);

typedef enum { WARNING_PRIORITY = 1, REGULAR_PRIORITY = 3 } TaskPriority;

typedef struct TaskControlBlock {
  TaskFunc func;
  TaskPriority priority;
  int is_available;
  int curr_num;
  struct QueueNode* my_node;
  struct TaskControlBlock* next_task;
  // task ID
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
 * @brief Simulate a task that gets executed right after short task gets
 * executed.
 *
 */
void dependent_task(TaskControlBlock* dependent_task_tcb);
