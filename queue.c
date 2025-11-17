#include <stdlib.h>

#include "local/linkedlist.h"
#include "local/queue.h"

int enqueue(queue_t *q, void *val) { return addAtTail(q->queue, &val); }

void *dequeue(queue_t *q) {
  void *val = getByIndex(q->queue, 0);
  deleteAtIndex(q->queue, 0);
  return val;
}

void freeQueue(queue_t *q) {
  freeLinkedList(q->queue);
  free(q);
}
