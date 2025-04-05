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
#include <time.h>
double time1, timedif;

void louvian_clustering(Graph *g){

    int number_of_edges  = edge_count(g);
    printf("edges: %d\n", number_of_edges);
    int n = g->n;
    int *communities = malloc( sizeof(int) * n);
    int tmp_community;

    time1 = (double) clock(); 

    time1 = time1 / CLOCKS_PER_SEC;
    for(int i = 0; i<n; i++) communities[i] = i;

    bool improvement = true;
    double best_modularity = -1;
    while(improvement){
        improvement = false;
        for(int i = 0; i<n; i++){
            int best_community = communities[i];
            for(int j = 0; j<g->nodes[i]->ne; j++){
                    tmp_community = communities[i];
                    int neighbor_comm = communities[g->nodes[i]->links[j]->id];

                    if (neighbor_comm == tmp_community)
                        continue; // no point in trying same community
                    communities[i] = neighbor_comm ;
                    double modularity = get_modularity(communities, g);
                    printf("modularity: %lf  best modularity %lf\n", modularity, best_modularity);
                    if(modularity>best_modularity){
                        printf("modularity has improved\n");
                        //printf("i: %d\n", i);
                        best_modularity = modularity;
                        improvement = true;
                        best_community = neighbor_comm;
                    }
                communities[i] = tmp_community;
            }
            communities[i] = best_community;
        }
    }

    timedif = ( ((double) clock()) / CLOCKS_PER_SEC) - time1;
    printf("The elapsed time is %f seconds\n", timedif);
    int communities_count = count_communities(communities, n);

    
    int *parts = malloc(sizeof(int)*communities_count);
    for (int i = 0; i < communities_count; i++) parts[i] = -1;

    int k = 0;
    for(int i = 0; i<n && k <communities_count; i++){
        int found = 0;
        for (int j = 0; j <k; j++){
            if (parts[j] == communities[i])
                found = 1;
        }
        if (!found)
            parts[k++] = communities[i];
    }
    for (int i = 0; i < communities_count; i++)
    printf("%d ", parts[i]);
        printf("\n");

    /*
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

    }*/


}

