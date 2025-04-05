#ifndef LOUVIAN_H
#define LOUVIAN_H

#include "graph.h"

// communities is a list of all nodes where node is the index and value represents certain community 
double get_modularity(int *communities, Graph *g);
void louvian_clustering(Graph *g);
double get_modularity(int *communities, Graph *g);

#endif