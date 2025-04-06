#ifndef LOUVIAN_H
#define LOUVIAN_H

#include "graph.h"

typedef struct List{
    int id;
    struct List *next;
}List;

// communities is a list of all nodes where node is the index and value represents certain community 
double get_modularity(int *communities, Graph *g);

void louvain(Graph *g, int desired_k);
void phase1(Graph *temporary_graph, Graph *original_graph);
Graph* phase2(Graph *g);

#endif