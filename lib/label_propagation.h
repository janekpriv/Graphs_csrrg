#ifndef LABEL_PROPAGATION_H
#define LABEL_PROPAGATION_H

#include "graph.h"
typedef struct{

    int label;
    int id;
    int *nne;

} node_propagation;

void print_communities(node_propagation *propagation, int n);
int find_most_frequent_value(int *arr, int n);
node_propagation *initiaize(Graph *g);
int get_most_frequent_label(int id, Graph *g, node_propagation *propagation);
void label_propagation(Graph *g);

#endif
