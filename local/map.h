#ifndef HEAP_H
#define HEAP_H

#include "linkedlist.h"

#define MAX_BUCKETS 128

typedef struct {
  char *key;
  void *val;
} keyval_t;

typedef struct {
  linkedlist_t *list;
  int size;
  int id;
} bucket_t;

typedef struct {
  bucket_t **buckets;
  int cnt;
} map_t;

void setToMap(map_t *map, char *key, void *val);

void *getFromMap(map_t *map, char *key);

bool existsInMap(map_t *map, char *key);

void deleteFromMap(map_t *map, char *key);

void freeMap(map_t *map);

#endif
