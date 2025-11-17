#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

#include "local/graph.h"

unsigned int addGraphNode(graph_t *graph, void *val) {
  if (!graph->nodes) {
    graph->nodes = malloc(sizeof(graph_node_t **));
    if (!graph->nodes) {
      return INT_MAX;
    }
  }
  if (!graph->matrix) {
    graph->matrix = malloc(sizeof(int *) * 1);
    if (!graph->matrix) {
      return INT_MAX;
    }
  }
  graph_node_t *node = malloc(sizeof(graph_node_t));
  graph->matrix = realloc(graph->matrix, sizeof(int *) * (graph->size + 1));
  graph->matrix[graph->size] = calloc(graph->size + 1, sizeof(int));
  if (!node || !graph->matrix || !graph->matrix[graph->size]) {
    return INT_MAX;
  }
  for (int i = 0; i < graph->size; i++) {
    graph->matrix[i] =
        realloc(graph->matrix[i], sizeof(int) * (graph->size + 1));
    if (!graph->matrix[i])
      return INT_MAX;
    graph->matrix[i][graph->size] = 0;
  }
  graph->nodes =
      realloc(graph->nodes, sizeof(graph_node_t *) * (graph->size + 1));
  if (!graph->nodes)
    return INT_MAX;
  node->id = graph->size;
  node->val = val;
  graph->nodes[graph->size] = node;
  graph->size++;
  return node->id;
}

void deleteGraphNode(graph_t *graph, const unsigned int id) {
  if (id > graph->size)
    return;
  graph_node_t *node = graph->nodes[id];
  free(node);
  graph->nodes[id] = NULL;
  for (int j = 0; j < graph->size; j++) {
    graph->matrix[id][j] = false;
    graph->matrix[j][id] = false;
  }
}

void addGraphRelation(graph_t *graph, const unsigned int fromNode,
                      const unsigned int toNode) {
  if (toNode > graph->size || fromNode > graph->size)
    return;
  if (!graph->nodes[fromNode] || !graph->nodes[toNode]) {
    return;
  }
  if (graph->matrix[fromNode][toNode])
    return;
  graph->matrix[fromNode][toNode] = true;
  graph->relations++;
}

void deleteGraphRelation(graph_t *graph, const unsigned int fromNode,
                         const unsigned int toNode) {
  if (toNode > graph->size || fromNode > graph->size)
    return;
  if (!graph->nodes[fromNode] || !graph->nodes[toNode]) {
    return;
  }
  if (!graph->matrix[fromNode][toNode])
    return;
  graph->matrix[fromNode][toNode] = false;
  graph->relations--;
}

bool existsGraphRelation(graph_t *graph, const unsigned int fromNode,
                         const unsigned int toNode) {
  if (toNode > graph->size || fromNode > graph->size)
    return false;

  if (!graph->nodes[fromNode] || !graph->nodes[toNode]) {
    return false;
  }
  return graph->matrix[fromNode][toNode];
}

void freeGraph(graph_t *graph) {
  for (int i = 0; i < graph->size; i++) {
    free(graph->nodes[i]);
    free(graph->matrix[i]);
  }
  free(graph->nodes);
  free(graph->matrix);
  free(graph);
}
