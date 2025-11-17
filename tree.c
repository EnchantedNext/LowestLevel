#include <stdbool.h>
#include <stdlib.h>

#include "local/tree.h"

node_t *getMappingNode(btree_t *tree, int val) {
  node_t *node = tree->root;
  while (true) {
    if (val > node->val) {
      if (!node->right) {
        return node;
      }
      node = node->right;
    } else if (val < node->val) {
      if (!node->left) {
        return node;
      }
      node = node->left;
    } else {
      return NULL;
    }
  }
}

int addToBTree(btree_t *tree, int val) {
  if (!tree->root) {
    node_t *root = malloc(sizeof(node_t));
    if (!root) {
      return -1;
    }
    root->val = val;
    root->left = NULL;
    root->right = NULL;
    tree->root = root;
    tree->length++;
    return 0;
  } else {
    node_t *parent = getMappingNode(tree, val);
    if (!parent) {
      return -1;
    }
    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node) {
      return -1;
    }
    new_node->val = val;
    new_node->left = NULL;
    new_node->right = NULL;
    if (val > parent->val) {
      parent->right = new_node;
    } else if (val < parent->val) {
      parent->left = new_node;
    }
    tree->length++;
    return 0;
  }
}

bool searchBTree(btree_t *tree, int val) {
  node_t *node = tree->root;
  while (node) {
    if (val > node->val) {
      node = node->right;
    } else if (val < node->val) {
      node = node->left;
    } else {
      return true;
    }
  }
  return false;
}

void freeBTree(node_t *root);

void freeBTree(node_t *root) {
  if (root) {
    freeBTree(root->right);
    freeBTree(root->left);
    free(root);
  }
}
