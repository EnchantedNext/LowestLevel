#ifndef QUEUE_H
#define QUEUE_H

#include "linkedlist.h"

typedef struct {
  linkedlist_t *queue;
} queue_t;

int enqueue(queue_t *q, void *val);

void *dequeue(queue_t *q);

void freeQueue(queue_t *q);

#endif
