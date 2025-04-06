#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>

#include "../lib/louvian.h"
#include "../lib/graph.h"

double get_modularity(int *communities, Graph *g){

    int n = g->n;
    int number_of_edges  = edge_count(g);

    double quality = 0;
    int k1, k2, A;
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            // check if node is in the same community 
            if(g->nodes[i]->comm == g->nodes[j]->comm){

                k1 = g->nodes[i]->ne;
                k2 = g->nodes[j]->ne;

                A = edge_exists(g, i, j);

                quality += (double)A - (double)(k1*k2)/(double)(2.0* number_of_edges);
                
                //printf("quality: %g, k1: %d, k2, %d\n", quality, k1, k2);

            }


        }
    }

    return  quality/(2.0*number_of_edges);

} 

int count_communities(Graph* g){
    int * unique_c = malloc(sizeof(int)* g->n);
    int c = 0;

    for(int i = 0; i<g->n; i++){
        int tmp = 0 ;
        for(int j = 0; j<c; j++){
            if(g->nodes[i]->comm == unique_c[j]){
                tmp = 1;
                break;
            }
        }
        if(tmp == 0){
            unique_c[c] = g->nodes[i]->comm;
            c++;
        }
    }
    free(unique_c);
    return c;

}

#include <time.h>
double time1, timedif;


void print_communities(Graph * g){
    int c = count_communities(g);
    int assigned_nodes = 0;
    printf("Total communities: %d\n", c);
    for (int i = 0; i < c; i++){
        printf("Community %d: ", i);
        int comm_size = 0;
        for (int j = 0; j < g->n; j++){
            if (g->nodes[j]->comm == i){
                printf(" %d ", j);
                comm_size++;
                assigned_nodes++;
            }
        }
        printf("(size: %d)\n", comm_size);
    }
    printf("Total assigned nodes: %d out of %d\n\n", assigned_nodes, g->n);
}

void louvain(Graph *g, int desired_k){
    bool changes = true;
    Graph *current_g = g;
    int min_communities = 2; 
    while (changes) {
        changes = false;
        printf("Starting phase1\n");
        phase1(current_g, g); // pass original graph for updates
        int current_count = count_communities(current_g);
        print_communities(g);
        fflush(stdout);
        print_list_repr(g);
        if (current_count <= desired_k || (current_count <= min_communities)) break;

        Graph *cg = phase2(current_g); // communities graph
        if (!cg) break;
        
        if (count_communities(cg) < current_count) { // check if aggregation reduced communities
            changes = true;
            if (current_g != g) free_graph(current_g); // free intermediate graph
            current_g = cg;
        } else {
            free_graph(cg); // no change, discard cg
            break;
        }
    }

    
    if (current_g != g) free_graph(current_g);
}

//change old community to new one
void change_communities(Graph *g, int old_comm, int new_comm) {
    for (int i = 0; i < g->n; i++) {
        if (g->nodes[i]->comm == old_comm) {
            g->nodes[i]->comm = new_comm;
        }
    }
}

// phase 1 of Louvain algorithm: local optimization

void phase1(Graph *g, Graph *og){
    int n = g->n;
    int *communities = malloc( sizeof(int) * n);
    int tmp_community;

    time1 = (double) clock(); 
    for(int i = 0; i<g->n; i++) g->nodes[i]->comm = i;
    time1 = time1 / CLOCKS_PER_SEC;
    int iteration = 0;
    
    bool improvement = true;
    double best_modularity = -1;
    while(improvement){
        improvement = false;
        iteration++;
        printf("Phase1 iteration %d\n", iteration);
        for(int i = 0; i<n; i++){
            int best_community = g->nodes[i]->comm;
            for(int j = 0; j<g->nodes[i]->ne; j++){
                    tmp_community = g->nodes[i]->comm;
                    int neighbor_comm = g->nodes[i]->links[j]->comm;
                    if (neighbor_comm == tmp_community)
                        continue; // no point in trying same community
                    g->nodes[i]->comm = neighbor_comm ;
                    double modularity = get_modularity(communities, g);
                    //printf("modularity: %lf  best modularity %lf\n", modularity, best_modularity);
                    if(modularity>best_modularity){
                        printf("modularity has improved: %lf\n", best_modularity);
                        best_modularity = modularity;
                        improvement = true;
                        best_community = neighbor_comm;
                    }
                g->nodes[i]->comm = tmp_community;   
            }
            g->nodes[i]->comm = best_community;
            
        }
    }
    printf("quality: %lf\n", best_modularity);
    int communities_count = count_communities(g);

    
    int *new_communities = malloc(sizeof(int)*communities_count);
    for (int i = 0; i < communities_count; i++) new_communities[i] = -1;
    
    int k = 0;
    for(int i = 0; i<n && k <communities_count; i++){
        int found = 0;
        for (int j = 0; j <k; j++){
            if (new_communities[j] == g->nodes[i]->comm){
                found = 1;
                break;
            }
        }
        if (!found)
        new_communities[k++] = g->nodes[i]->comm;
    }

    
    for(int i = 0; i < og->n ; i++){
        bool changed = false;
        for (int j = 0; j <communities_count; j++){
            if (new_communities[j] == og->nodes[i]->comm){
                og->nodes[i]->comm = j;
                changed = true;
                break;
            }
        }
        if (!changed){
            og->nodes[i]->comm = g->nodes[og->nodes[i]->comm+1]->comm;
            i--;
        }
        
    }

    if (og != g){
        for(int i = 0; i < g->n ; i++){
            for (int j = 0; j <communities_count; j++){
                if (new_communities[j] == g->nodes[i]->comm){
                    g->nodes[i]->comm = j;
                    break;
                }
            }
        }
    }

    timedif = ( ((double) clock()) / CLOCKS_PER_SEC) - time1;
    printf("The elapsed time is %f seconds\n", timedif);

}


// phase 2 of Louvain algorithm: aggregation
Graph *phase2(Graph *g){
    int n = g->n;
    int communities_count = count_communities(g);
    if (communities_count <= 1) return NULL;
    // graph where every node is a community
    Graph * cg = graph_init(communities_count, UNDIRECTED);

    int c = 0; 
    for(int i = 0; i<n ; i++){
        int comm1 = g->nodes[i]->comm;
        for (int j = 0; j< g->nodes[i]->ne; j++){
            int comm2 = g->nodes[i]->links[j]->comm;
            if (comm1 != comm2 ){
                    c += add_node(cg,comm1, comm2, c);
                    //printf("adding %d - %d\n", comm1, comm2);
                
            }

        }
        
    }
    sort_graph(cg);
    
    return cg;

}
