#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "../lib/louvian.h"
#include "../lib/graph.h"

#define MAX_ITERATIONS 10000
#define MODULARITY_THRESHOLD 0.00001

void shuffle(int *array, int n) {
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

int count_edges_to_community(Graph *g, int node, int *communities, int target_community) {
    int count = 0;
    for (int i = 0; i < g->nodes[node]->ne; i++) {
        int neighbor = g->nodes[node]->links[i]->id;
        if (communities[neighbor] == target_community) {
            count++;
        }
    }
    return count;
}

double get_delta_modularity(Graph *g, int node, int target_comm, int *communities, int *comm_degrees, int m) {
    int ki = g->nodes[node]->ne;
    int ki_in = count_edges_to_community(g, node, communities, target_comm);
    int sum_tot = comm_degrees[target_comm];

    return ((double)ki_in / m) - ((double)ki * sum_tot) / (2.0 * m * m);
}

double get_modularity(Graph *g, int *communities, int *comm_degrees) {
    int m = edge_count(g);
    double modularity = 0.0;

    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->nodes[i]->ne; j++) {
            int neighbor = g->nodes[i]->links[j]->id;
            if (communities[i] == communities[neighbor]) {
                modularity += 1.0 - ((double)g->nodes[i]->ne * g->nodes[neighbor]->ne) / (2.0 * m);
            }
        }
    }

    return modularity / (2.0 * m);
}

void louvian_clustering(Graph *g) {
    int n = g->n;
    int m = edge_count(g);
    int *communities = malloc(sizeof(int) * n);
    int *comm_degrees = calloc(n, sizeof(int));
    int *indices = malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++) {
        communities[i] = i;
        comm_degrees[i] = g->nodes[i]->ne;
        indices[i] = i;
    }

    double prev_mod = -1.0;
    int iterations = 0;

    while (iterations < MAX_ITERATIONS) {
        bool changed = false;
        shuffle(indices, n);
        //adding randomness to the algorithm, to not get stuck in the local maximum 
        

        for (int ii = 0; ii < n; ii++) {
            int i = indices[ii];
            int best_comm = communities[i];
            double best_gain = 0.0;

            for (int j = 0; j < g->nodes[i]->ne; j++) {
                int neighbor = g->nodes[i]->links[j]->id;
                int target_comm = communities[neighbor];
                if (target_comm == communities[i]) continue;

                double delta_q = get_delta_modularity(g, i, target_comm, communities, comm_degrees, m);
                if (delta_q > best_gain) {
                    best_gain = delta_q;
                    best_comm = target_comm;
                }
            }

            if (best_comm != communities[i]) {
                comm_degrees[communities[i]] -= g->nodes[i]->ne;
                communities[i] = best_comm;
                comm_degrees[best_comm] += g->nodes[i]->ne;
                changed = true;
            }
        }

        double new_mod = get_modularity(g, communities, comm_degrees);
        // if (!changed || new_mod - prev_mod < MODULARITY_THRESHOLD) break;
        // with this on there are only 3 iterations

        prev_mod = new_mod;
        iterations++;
    }

    printf("algorithm ended after %d iterations\n", iterations);

    int communities_count = count_communities(communities, n);
    printf("Graph has been partitioned to %d communities:\n", communities_count);

    int *partitioned_communities = unique_communities(g, communities, communities_count);

    for (int i = 0; i < communities_count; i++) {
        printf("Community %d:\n", i+1);
        for (int j = 0, c = 0; j < n; j++) {
            if (communities[j] == partitioned_communities[i]) {
                printf("%d ", j);
                if (++c % 10 == 0) printf("\n");
            }
        }
        printf("\n");
    }

    free(communities);
    free(comm_degrees);
    free(indices);
    free(partitioned_communities);
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
