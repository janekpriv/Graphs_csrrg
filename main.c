#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"./lib/parse_csr.h"
#include "./lib/graph.h"
#include "./lib/louvian.h"

int main(int argc, char **argv){
    Graph *g = generate_graph_from_csrrg(argv[1]);
    if (!g){
        perror("Error generating initial graph");
        return -1;
    }
    puts("Finished generation of initial graph");
    
    // Convert the number of partitions if it's correct, otherwise set it to 2.
    long int p; char *end_ptr;
    int parts = argc > 2 ? (p = (strtol(argv[2], &end_ptr, 10))) != 0L && p < (g->n)/4 ? (int)p : 2  : 2;
    
    sort_graph(g); 

    louvain(g, parts);
    print_communities(g);
    
    print_list_repr(g);
    free_graph(g);
}