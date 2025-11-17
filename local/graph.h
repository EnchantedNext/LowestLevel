#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
  int id;
  void *val;
} graph_node_t;

typedef struct {
  int size;
  int relations;
  int **matrix;
  graph_node_t **nodes;
} graph_t;

unsigned int addGraphNode(graph_t *graph, void *val);

void deleteGraphNode(graph_t *graph, const unsigned int id);

void addGraphRelation(graph_t *graph, const unsigned int fromNode,
                      const unsigned int toNode);

void deleteGraphRelation(graph_t *graph, const unsigned int fromNode,
                         const unsigned int toNode);

bool existsGraphRelation(graph_t *graph, const unsigned int fromNode,
                         const unsigned int toNode);

void freeGraph(graph_t *graph);

#endif
