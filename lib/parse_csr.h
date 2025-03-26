#ifndef PARSE_CSR_H
#define PARSE_CSR_H

void parse(char* filename);
Graph *generate_graph_from_csrr(char* filname);
int get_number_of_nodes(char *a_r);
int *tokenize(char *edges_offset, int c_count);
void convert(char *edges, int *e_offset, Graph* g);

#endif