#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"./lib/parse_csr.h"
#include "./lib/graph.h"
#include "./lib/louvian.h"

int main(int argc, char **argv){

    Graph *g = generate_graph_from_csrrg(argv[1]);
    // for(int i = 0; i<g->n; i++){
    //     printf("Node: %d, number of nieghbours: %d\n", i, g->nodes[i]->ne);
    // }
    louvian_clustering(g);
    sort_graph(g);
    print_list_repr(g);
    printf("test\n");

}