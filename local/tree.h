#ifndef TREE_H
#define TREE_H

typedef struct node_t node_t;

struct node_t {
  int val;
  node_t *left;
  node_t *right;
};

typedef struct {
  node_t *root;
  int length;
} btree_t;

#endif
