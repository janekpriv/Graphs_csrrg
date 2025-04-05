#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<time.h>

#include "../lib/louvian.h"
#include "../lib/graph.h"

#define MAX_ITERATIONS 100000
#define MIN_ITERATIONS 100  // Limit iteracji
#define MODULARITY_THRESHOLD 0.0001  // Minimalna poprawa modularności


void shuffle(int *array, int n) {
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}


double get_delta_modularity(Graph *g, int new_community, int *community_degrees, int number_of_edges, int *communities, int id) {
    int old_community = communities[id];

    if (old_community == new_community) return 0;

    int k = g->nodes[id]->ne;  
    int k_new = community_degrees[new_community];
    int k_old = community_degrees[old_community];

    double quality = (double)(k_new - k_old) / (2 * number_of_edges);
    return quality;
}

// Funkcja obliczająca modularność grafu
double get_modularity(int *communities, Graph *g, int *community_degrees) {
    int n = g->n;
    int number_of_edges = edge_count(g);
    double quality = 0.0;

    for (int i = 0; i < n; i++) {
        int k1 = g->nodes[i]->ne;
        for (int j = 0; j < g->nodes[i]->ne; j++) {
            int neighbour = g->nodes[i]->links[j]->id;
            if (communities[i] == communities[neighbour]) {
                quality += 1.0 - ((double)(k1 * community_degrees[communities[neighbour]]) / (2.0 * number_of_edges));
            }
        }
    }

    return quality / (2.0 * number_of_edges);
}

// Implementacja algorytmu Louvain
void louvian_clustering(Graph *g) {
    int number_of_edges = edge_count(g);
    int n = g->n;
    int *communities = malloc(sizeof(int) * n);
    int *community_degrees = malloc(sizeof(int) * n);
    int *indices = malloc(sizeof(int)*n);

    for (int i = 0; i < n; i++) indices[i] = i;
    
    for (int i = 0; i < n; i++) {
        communities[i] = i;
        community_degrees[i] = g->nodes[i]->ne;
        printf("community degree: %d\n", community_degrees[i]);
    }

    bool improvement = true;
    int iterations = 0;
    double prev_modularity = -1.0;
    srand(time(NULL));

    while (improvement && iterations<MAX_ITERATIONS) {
        improvement = false;
        double modularity = get_modularity(communities, g, community_degrees);
        shuffle(indices, n);

        for (int in = 0; in < n; in++) {
            int i = indices[in];
            int best_community = communities[i];
            double best_modularity = -1;

            for (int j = 0; j < g->nodes[i]->ne; j++) {
                int neighbour = g->nodes[i]->links[j]->id;
                int tmp_community = communities[neighbour];

                double local_modularity = get_delta_modularity(g, tmp_community, community_degrees, number_of_edges, communities, i);
                modularity = get_modularity(communities, g, community_degrees);
                if (modularity > best_modularity || (rand() % 10 < 2)) {
                    best_modularity = local_modularity;
                    best_community = tmp_community;
                    improvement = true;
                }
            }
            //printf("Before: Community %d has degree %d\n", communities[i], community_degrees[communities[i]]);
            
            if (best_community != communities[i]) {
                
                community_degrees[communities[i]] -= g->nodes[i]->ne;
                communities[i] = best_community;
                community_degrees[best_community] += g->nodes[i]->ne;
            }
            //printf("After: Community %d has degree %d\n", best_community, community_degrees[best_community]);
        }


        double new_modularity = get_modularity(communities, g, community_degrees);
        // if (new_modularity - prev_modularity < MODULARITY_THRESHOLD) {
        //     break;
        // }
        prev_modularity = new_modularity;
        iterations++;
    }
    printf("algorithm ended after %d iterations\n", iterations);

    int communities_count = count_communities(communities, n);
    printf("Communities count: %d\n",communities_count);

    printf("Graph has been partitioned to %d communities:\n", communities_count);

    int *partitioned_communities = unique_communities(g, communities, communities_count);

    for(int i = 0; i<communities_count; i++){

        int c = 0;
        printf("Community %d:\n", i);
        
        for(int j = 0; j<n; j++){
            
            if(communities[j] == partitioned_communities[i]){
                
                
                if(c==10){
                    printf("\n");
                    c= 0;
                }
                printf("%d ", j);
                c++;
            }
        }
        printf("\n");

    }

    free(communities);
    free(community_degrees);
}
int count_communities(int *communities, int n_number){

    int * unique_c = malloc(sizeof(int)* n_number);
    int c = 0;

    for(int i = 0; i<n_number; i++){
        int tmp = 0 ;
        for(int j = 0; j<n_number; j++){
            if(communities[i] == unique_c[j]){
                tmp = 1;
                break;
            }
        }
        if(tmp == 0){
            unique_c[c] = communities[i];
            c++;
        }
    }
    free(unique_c);
    return c;

}

int *unique_communities(Graph *g, int *communities, int communities_count){

    int *partitioned_communities = malloc(sizeof(int)* communities_count);
    int c =0;

    for(int i = 0; i<g->n; i++){
        int is_unique =1;
        for(int j = 0; j<communities_count; j++){
            if(communities[i]==partitioned_communities[j]){
                is_unique = 0;
                break;
            }
        }

        if(is_unique){
            partitioned_communities[c++] = communities[i];
        }
    }

    return partitioned_communities;
}

