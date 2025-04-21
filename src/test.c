#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "parse_csr.h"
#include "graph.h"
#include "louvian.h"
void print_graph_debug(Graph *g) {
    printf("Graph Debug Info:\n");
    for (int i = 0; i < g->n; i++) {
        Node u = g->nodes[i];
        printf("Node %d (comm: %d):\n", u->id, u->comm);
        
        for (int j = 0; j < u->ne; j++) {
            Node v = u->links[j];
            int weight = 1; // default for unweighted
            if (u->edges != NULL && u->edges[j] != NULL)
                weight = u->edges[j]->weight;

            if (u->id == v->id) {
                printf("  ↻ Self-loop (weight: %d)\n", weight);
            } else {
                printf("  → Node %d (weight: %d)\n", v->id, weight);
            }
        }
    }
}

void test_lil_graph(){
    Graph * g2 = graph_init(9, UNDIRECTED);
    int c = 0;
    int values_map[] = {0, 1, 0, 4, 4, 1, 1, 2, 2, 3, 3,
                 4, 4, 5, 5, 8, 5, 6, 6, 8, 6, 7, 7, 8};
    for (int i = 0; i < 24; i +=2){
        int n1 = values_map[i];
        int n2 = values_map[i+1];
        c += add_node(g2, n1, n2, c, 1);
    }
    sort_graph(g2);
    /* manual 
    phase1(g2, g2, 3);
    print_communities(g2);
     double mod = get_modularity(NULL, g2);
    printf("%lf\n", mod);  // 0.47826086956521735

    Graph *ng = phase2(g2);
    print_graph_debug(ng);

    phase1(ng, g2, 3);
    mod = get_modularity(NULL, g2);
    printf("%lf\n", mod);

    print_communities(g2); 
    */
    louvain(g2);
    print_communities(g2); 
    
}

void test_phases(int n){
    if (!(n < 5) || !(n >= 0))
        n = 0;
    char filename[30];
    sprintf(filename, "./input/graf%d.csrrg", n);
    Graph *g = generate_graph_from_csrrg(filename);
    if (!g){
        perror("Error generating initial graph");
        return;
    }
    puts("Finished generation of initial graph");

    sort_graph(g);
    //print_list_repr(g);
    //system("gedit output/graph.txt");

    // Modularity test for the one community: mod close to 0 (good).
    double mod = get_modularity(g);
    printf("%lf\n", mod); // m = 0.0090326340326345005

    // Modularity test for the n communities: mod is 0 (good).
    seed_communities(g);
    mod = get_modularity(g);
    printf("%lf\n", mod); // m = 0.0 

    // 4 iterations
    phase1(g, g, mod);
    print_communities(g);
    //mod = get_modularity(NULL, g);
    //printf("%lf\n", mod); 
    /*RESULTS
    iter = 4 : m = 0.29441854441854426
    iter = 10 : m = 0.31614219114219072
    iter = 100 : m = 0.26295001295001302
    iter = 1 : m = 0.30052123802123787
    iter = 2 : m = 0.29524410774410764
    iter = 1000 : m = 0.2607161357161355
    iter = 10 000 : m = 0.18176961926961985
    */

    Graph *ng = phase2(g);
    
    // 2 run
    phase1(ng, g, mod);
    mod = get_modularity(g);
    printf("%lf\n", mod);

    ng = phase2(g);
    //print_graph_debug(ng);

    phase1(ng, g, mod);
    mod = get_modularity(g);
    printf("%lf\n", mod);

    print_communities(g); 
}

void test_louvain(int n){
    if (!(n <= 5) || !(n >= 0))
    n = 0;
    char filename[30];
    sprintf(filename, "./input/graf%d.csrrg", n);
    Graph *g = generate_graph_from_csrrg(filename);
    if (!g){
        perror("Error generating initial graph");
        return;
    }
    puts("Finished generation of initial graph");

    sort_graph(g);
    int ncomm = 0;
    ncomm = louvain(g);
    if (ncomm != 3)
     merge_to_three_communities(g, ncomm);
    printf("Final modularity: %lf\n", get_modularity(g));
}

int main(int argc, char **argv){
    //test_lil_graph();
    int test_number = argc == 2 ? atoi(argv[1]) : 0;
    test_louvain(test_number);


    
}

/*
Changed og values to new communities
The elapsed time is 4.353125 seconds
Starting phase 2
New modularity: 0.426951
Community count decreased: 1464 -> 745
Starting phase 1
Finished optimization loop
Counted communities
Changed og values to new communities
The elapsed time is 4.395140 seconds
Starting phase 2
New modularity: 0.442137
Community count decreased: 745 -> 229
Starting phase 1
Finished optimization loop
Counted communities
Changed og values to new communities
The elapsed time is 0.728086 seconds
Starting phase 2
New modularity: 0.444656
Community count decreased: 229 -> 62
Starting phase 1
Finished optimization loop
Counted communities
Changed og values to new communities
The elapsed time is 0.011927 seconds
Starting phase 2
New modularity: 0.240949
Community count 62 -> 62
COMM COUNT INITIALLY = 62Louvain Algorithm has terminated
Total communities: 20
Final modularity: 0.240949*/


/*
NG
Changed og values to new communities
The elapsed time is 8.783390 seconds
0.387660
Finished optimization loop
Counted communities
Changed og values to new communities
The elapsed time is 3.510524 seconds
0.424472
Total communities: 747

G
inished optimization loop
Counted communities
Changed og values to new communities
The elapsed time is 3.937438 seconds
0.460960
Total communities: 730
*/