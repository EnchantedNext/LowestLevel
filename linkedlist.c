#include <stdlib.h>

#include "local/linkedlist.h"

int addHeadToEmpty(linkedlist_t *list, void *val) {
  list->length++;
  node_t *head = malloc(sizeof(node_t));
  if (head == NULL) {
    return -1;
  }
  head->val = val;
  head->next = NULL;
  list->head = head;
  list->tail = head;
  return 0;
}

int addAtTail(linkedlist_t *list, void *val) {
  if (!list->length) {
    return addHeadToEmpty(list, val);
  }
  node_t *node = malloc(sizeof(node_t));
  if (node == NULL) {
    return -1;
  }
  node->val = val;
  node->next = NULL;
  list->tail->next = node;
  list->tail = list->tail->next;
  list->length++;
  return 0;
}

int addAtHead(linkedlist_t *list, void *val) {
  if (!list->length) {
    return addHeadToEmpty(list, val);
  }
  node_t *node = malloc(sizeof(node_t));
  if (node == NULL) {
    return -1;
  }
  node->val = val;
  node->next = list->head;
  list->head = node;
  list->length++;
  return 0;
}

node_t *findNode(linkedlist_t *list, int idx) {
  node_t *node = list->head;
  for (int i = 0; i < idx; i++) {
    node = node->next;
  }
  return node;
}

int addAtIndex(linkedlist_t *list, void *val, int idx) {
  if (idx > list->length) {
    return -1;
  }
  if (idx == -1) {
    return addAtTail(list, val);
  } else if (idx == 0) {
    return addAtHead(list, val);
  }
  node_t *node = findNode(list, idx - 1);
  node_t *new_node = malloc(sizeof(node_t));
  if (new_node == NULL) {
    return -1;
  }
  new_node->val = val;
  new_node->next = node->next;
  node->next = new_node;
  list->length++;
  return 0;
}

void *getByIndex(linkedlist_t *list, int idx) {
  if (idx >= list->length || idx < -1) {
    return NULL;
  } else if (idx == 0) {
    return list->head->val;
  } else if (idx == -1) {
    return list->tail->val;
  }
  node_t *node = findNode(list, idx);
  return node->val;
}

int deleteAtIndex(linkedlist_t *list, int idx) {
  if (!list->length) {
    return -1;
  }
  if (idx >= list->length || idx < -1) {
    return -1;
  }
  if (idx == 0) {
    node_t *toDelete = list->head;
    list->head = list->head->next;
    if (!list->head) {
      list->tail = NULL;
    }
    list->length--;
    free(toDelete);
    return 0;
  } else if (idx == -1 || idx == list->length - 1) {
    node_t *node = findNode(list, list->length - 2);
    node_t *next = node->next;
    node->next = NULL;
    list->tail = node;
    list->length--;
    free(next);
    return 0;
  }
  node_t *node = findNode(list, idx - 1);
  node_t *next = node->next;
  node->next = node->next->next;
  list->length--;
  free(next);
  return 0;
}

void freeLinkedList(linkedlist_t *list) {
  node_t *node = list->head;
  node_t *next;
  while (node != NULL) {
    next = node->next;
    free(node);
    node = next;
  }
  list->head = NULL;
  list->tail = NULL;
  list->length = 0;
}
