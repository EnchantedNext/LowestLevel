#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "local/linkedlist.h"
#include "local/map.h"

#define MAX_BUCKETS 128

int hash_f(const char *key) {
  unsigned long hash = 5381;
  int c;

  while ((c = *key++))
    hash = ((hash << 5) + hash) + c;

  return hash % MAX_BUCKETS;
}

void setToMap(map_t *map, char *key, void *val) {
  if (!map->buckets) {
    map->buckets = calloc(MAX_BUCKETS, sizeof(bucket_t *));
  }
  int bucket_id = hash_f(key);
  bucket_t *bucket = map->buckets[bucket_id];
  if (!bucket) {
    bucket_t *new_bucket = malloc(sizeof(bucket_t));
    linkedlist_t *list = malloc(sizeof(linkedlist_t));
    keyval_t *kv = malloc(sizeof(keyval_t));
    if (!new_bucket || !list || !kv) {
      free(new_bucket);
      free(list);
      free(kv);
      return;
    }
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    map->cnt++;
    new_bucket->id = map->cnt;
    new_bucket->list = list;
    kv->key = key;
    kv->val = val;
    addAtTail(new_bucket->list, kv);
    new_bucket->size = new_bucket->list->length;
    map->buckets[bucket_id] = new_bucket;
    return;
  } else {
    keyval_t *kv = malloc(sizeof(keyval_t));
    if (!kv) {
      return;
    }
    kv->key = key;
    kv->val = val;
    addAtTail(bucket->list, kv);
    bucket->size = bucket->list->length;
    return;
  }
}

void *getFromMap(map_t *map, char *key) {
  if (!map->buckets) {
    return NULL;
  }
  int bucket_id = hash_f(key);
  bucket_t *bucket = map->buckets[bucket_id];
  if (!bucket) {
    return NULL;
  }
  node_t *node = bucket->list->head;
  while (node != NULL) {
    keyval_t *kv = (keyval_t *)node->val;
    if (!strcmp(kv->key, key)) {
      return kv->val;
    }
    node = node->next;
  }
  return NULL;
}

bool existsInMap(map_t *map, char *key) {
  if (!map->buckets) {
    return false;
  }
  int bucket_id = hash_f(key);
  bucket_t *bucket = map->buckets[bucket_id];
  if (!bucket) {
    return false;
  }
  node_t *node = bucket->list->head;
  while (node != NULL) {
    keyval_t *kv = (keyval_t *)node->val;
    if (!strcmp(kv->key, key)) {
      return true;
    }
    node = node->next;
  }
  return false;
}

void deleteFromMap(map_t *map, char *key) {
  if (!map->buckets) {
    return;
  }
  int bucket_id = hash_f(key);
  bucket_t *bucket = map->buckets[bucket_id];
  if (!bucket) {
    return;
  }
  node_t *node = bucket->list->head;
  int idx = 0;
  while (node != NULL) {
    keyval_t *kv = (keyval_t *)node->val;
    if (!strcmp(kv->key, key)) {
      deleteAtIndex(bucket->list, idx);
      return;
    }
    node = node->next;
    idx++;
  }
  return;
}

void freeMap(map_t *map) {
  if (!map) {
    return;
  }
  for (int i = 0; i < map->cnt; i++) {
    bucket_t *bucket = map->buckets[i];
    if (!bucket) {
      continue;
    }
    node_t *node = bucket->list->head;
    node_t *next;
    while (node != NULL) {
      next = node->next;
      keyval_t *kv = (keyval_t *)node->val;
      free(kv->key);
      free(kv->val);
      free(kv);
      free(node);
      node = next;
    }
    free(bucket->list);
    free(bucket);
  }
  free(map->buckets);
  free(map);
}
