#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>

#include "../lib/louvian.h"
#include "../lib/graph.h"


// compute local modularity to optimize runtime
// currently not used
int compute_community_sum(Graph *g, int *communities, int community){

    int community_sum = 0;
    for(int i = 0; i<g->n; i++){
        if(communities[i] == community){
            community_sum+=g->nodes[i]->ne;
        }
    }

}


double get_delta_modularity(Graph *g, int new_community, int *degrees, int edge_count, int *communities, int id){

    int old_community = communities[id];

    if(old_community == new_community) return 0;

    int k = g->nodes[id]->ne;
    int k_new = degrees[new_community];
    int k_old = degrees[old_community];

    //int e_sum_old = compute_community_sum(g, communities, new_community);
    //int e_sum_new = compute_community_sum(g, communities, new_community);

    
    //double quality = (double)(e_sum_old - k_new - e_sum_new + k_old)/(2*edge_count);
    double quality = (double)(k_new - k_old) / (2 * edge_count);
    printf("old community: %d new community: %dk_new:%d k_old:%d k: %d quality: %g\n",old_community, new_community, k_new, k_old, k, quality);

    return quality;

}




double get_modularity(int *communities, Graph *g){

    int n = g->n;

    int number_of_edges  = edge_count(g);

    double quality = 0;
    int k1, k2, A;
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            // check if node is in the same community 
            if(communities[i] == communities[j]){

                k1 = g->nodes[i]->ne;
                k2 = g->nodes[j]->ne;

                A = edge_exists(g, i, j);

                quality += (double)A - (double)(k1*k2)/(double)(2.0* number_of_edges);

                //printf("quality: %g, k1: %d, k2, %d\n", quality, k1, k2);

            }


        }
    }

    double modularity = quality/(2.0*number_of_edges);

    printf("Modularity: %g\n", modularity);

    return modularity;

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
    return c;

}

void louvian_clustering(Graph *g){

    int number_of_edges  = edge_count(g);
    printf("edges: %d\n", number_of_edges);
    int n = g->n;
    int *communities = malloc( sizeof(int) * n);
    int tmp_community;

    int *degrees = malloc(sizeof(int)*n);
        for(int i = 0; i<n; i++){
            degrees[i] = g->nodes[i]->ne;
            printf("node %d has %d degrees\n", i, degrees[i]);
        }
    


    for(int i = 0; i<n; i++) communities[i] = i;

    bool improvement = true;

    while(improvement){
        improvement = false;
        for(int i = 0; i<n; i++){
            int best_community = communities[i];
            double best_modularity = -1;
            for(int j = 0; j<g->nodes[i]->ne; j++){
                //if(edge_exists(g, i,j) && i != j){
                    int neighbour = g->nodes[i]->links[j]->id;
                    tmp_community = communities[neighbour];

                    double local_modularity = get_delta_modularity(g, tmp_community, degrees, number_of_edges, communities, i);
                    double modularity = get_modularity(communities, g);
                    printf("modularity: %g\n", local_modularity);
                    if(modularity>best_modularity){
                       // printf("modularity has improved\n");
                        // printf("i: %d\n", i);
                        //printf("communities %d node %d\n", communities[i],i);
                        best_modularity = local_modularity;
                        improvement = true;
                        best_community = tmp_community;
                    }
                //}
                //communities[i] = tmp_community;

            }
            if (best_community != communities[i]) {
                if (degrees[communities[i]] >= g->nodes[i]->ne) {  
                    degrees[communities[i]] -= g->nodes[i]->ne;
                }
            
                communities[i] = best_community;
            
                degrees[best_community] += g->nodes[i]->ne;
            }
            //communities[i] = best_community;
        }
    }

    int communities_count = count_communities(communities, n);

    for(int i = 0; i<communities_count; i++){

        printf("Graph has been partitioned to %d communities:\n", communities_count);
        printf("Community %d:\n");
        for(int j = 0; j<n; j++){
            if(communities[j] == i){
                int c = 0;
                if(c==10){
                    printf("\n");
                }
                printf("%d ", j);
                c++;
            }
        }

    }


}

