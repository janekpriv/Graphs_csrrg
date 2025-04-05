#ifndef LOUVIAN_H
#define LOUVIAN_H

#include "graph.h"

// communities is a list of all nodes where node is the index and value represents certain community 
double get_modularity(int *communities, Graph *g, int*);
double get_delta_modularity(Graph *g, int new_community, int *degrees, int edge_count, int *communities, int id);
void louvian_clustering(Graph *g);
int *unique_communities(Graph *g, int *communities, int communities_count);

#endif