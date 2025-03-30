#ifndef PARSE_CSR_H
#define PARSE_CSR_H

#include "graph.h"

void parse(char* filename);
Graph *generate_graph_from_csrrg(char* filname);
int get_number_of_nodes(char *a_r);
int *tokenize(char *edges_offset, int len);
void convert(int *e_id, int *edges, int e_count, Graph *g);

#endif