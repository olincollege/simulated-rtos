typedef struct QueueNode {
  TaskControlBlock* task;
  struct QueueNode* next;
} QueueNode;

typedef struct {
  QueueNode* front;  // pointer to the first element in queue
  QueueNode* last;   // pointer to the last task in queue
  int length;        // length of queue
} Queue;
void enqueue(Queue* q, QueueNode new_task);
