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

    //printf("Modularity: %g\n", modularity);

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

    for(int i = 0; i<n; i++) communities[i] = i;

    bool improvement = true;

    while(improvement){
        improvement = false;
        for(int i = 0; i<n; i++){
            int best_community = communities[i];
            double best_modularity = -1;
            for(int j = 0; j<n; j++){
                if(edge_exists(g, i,j)){
                    tmp_community = communities[i];
                    communities[i] = communities[j];

                    double modularity = get_modularity(communities, g);
                    //printf("modularity: %d\n", modularity);
                    if(modularity>best_modularity){
                        //printf("modularity has improved\n");
                        printf("i: %d\n", i);
                        best_modularity = modularity;
                        improvement = true;
                        best_community = communities[i];
                    }
                }
                communities[i] = tmp_community;
            }
            communities[i] = best_community;
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

