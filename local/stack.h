#ifndef STACK_H
#define STACK_H

#include "linkedlist.h"

typedef struct {
  linkedlist_t stack;
} stack_t;

int push(stack_t *st, int val);

int pop(stack_t *st);

int peek(stack_t *st);

void freeStack(stack_t *st);

#endif
