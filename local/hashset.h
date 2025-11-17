#ifndef HASHSET_H
#define HASHSET_H

#include <stdbool.h>

#include "linkedlist.h"

typedef struct {
  linkedlist_t *list;
  int size;
  int id;
} bucket_t;

typedef struct {
  bucket_t **buckets;
  int cnt;
} hashset_t;

void hashSetAdd(hashset_t *set, void *val);

void hashSetDelete(hashset_t *set, void *val);

bool hashSetExists(hashset_t *set, void *val);

void freeHashSet(hashset_t *set);

#endif
