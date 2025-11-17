#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "local/hashset.h"

#define MAX_BUCKETS 128

static inline size_t ptrHash(const void *ptr) {
  uintptr_t x = (uintptr_t)ptr;
  x ^= x >> 33;
  x *= 0xff51afd7ed558ccdULL;
  x ^= x >> 33;
  x *= 0xc4ceb9fe1a85ec53ULL;
  x ^= x >> 33;
  return (size_t)x;
}

static inline size_t bucketIndex(const void *ptr) {
  int id = (int)(ptrHash(ptr) % MAX_BUCKETS);
  return id;
}

void hashSetAdd(hashset_t *set, void *val) {
  if (!set->buckets) {
    set->buckets = calloc(MAX_BUCKETS, sizeof(bucket_t *));
    if (!set->buckets) {
      return;
    }
  }
  int bucket_id = bucketIndex(val);
  bucket_t *bucket = set->buckets[bucket_id];
  if (!bucket) {
    bucket_t *new_bucket = malloc(sizeof(bucket_t));
    linkedlist_t *list = malloc(sizeof(linkedlist_t));
    if (!new_bucket || !list) {
      free(new_bucket);
      free(list);
      return;
    }
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    set->cnt++;
    new_bucket->id = set->cnt;
    new_bucket->list = list;
    addAtTail(new_bucket->list, val);
    new_bucket->size = new_bucket->list->length;
    set->buckets[bucket_id] = new_bucket;
    return;
  } else {
    addAtTail(bucket->list, val);
    bucket->size = bucket->list->length;
    return;
  }
}

void hashSetDelete(hashset_t *set, void *val) {
  if (!set->buckets) {
    set->buckets = calloc(MAX_BUCKETS, sizeof(bucket_t *));
    return;
  }
  int bucket_id = bucketIndex(val);
  bucket_t *bucket = set->buckets[bucket_id];
  if (!bucket) {
    return;
  }
  node_t *node = bucket->list->head;
  int idx = 0;
  while (node != NULL) {
    if (node->val == val) {
      deleteAtIndex(bucket->list, idx);
      return;
    }
    idx++;
    node = node->next;
  }
}

bool hashSetExists(hashset_t *set, void *val) {
  if (!set->buckets) {
    set->buckets = calloc(MAX_BUCKETS, sizeof(bucket_t *));
    return false;
  }
  int bucket_id = bucketIndex(val);
  bucket_t *bucket = set->buckets[bucket_id];
  if (!bucket) {
    return false;
  }
  node_t *node = bucket->list->head;
  int idx = 0;
  while (node != NULL) {
    if (node->val == val) {
      return true;
    }
    idx++;
    node = node->next;
  }
  return false;
}

void freeHashSet(hashset_t *set) {
  if (!set) {
    return;
  }
  for (int i = 0; i < set->cnt; i++) {
    bucket_t *bucket = set->buckets[i];
    if (!bucket) {
      continue;
    }
    node_t *node = bucket->list->head;
    node_t *next;
    while (node != NULL) {
      next = node->next;
      free(node);
      node = next;
    }
    free(bucket->list);
    free(bucket);
  }
  free(set->buckets);
  free(set);
}
