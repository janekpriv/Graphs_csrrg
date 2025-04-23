#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"../lib/parse_csr.h"
#include "../lib/graph.h"
#include "../lib/louvian.h"



int main(int argc, char **argv){
    Graph *g = generate_graph_from_csrrg(argv[1]);
    if (!g){
        perror("Error generating initial graph ");
        return -1;
    }
    sort_graph(g); 
    //puts("Finished generation of initial graph");
    
    
    int ncomm = 0;
    ncomm = louvain(g);
    if (ncomm != 3)
        merge_to_three_communities(g);
    printf("Final modularity: %lf\n", get_modularity(g));
    //print_communities(g);
    save_list_repr(g);
    free_graph(g);
}


