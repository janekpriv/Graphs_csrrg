#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#include "../lib/graph.h"
#include "../lib/label_propagation.h"

#define MAX_ITERATIONS 100


void shuffle(int *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}

void print_communities(node_propagation *propagation, int n){

    int * unique_c = malloc(sizeof(int)* n);
    int c = 0;
    int e = 0;

    for(int i = 0; i<n; i++){
        int tmp = 0 ;
        for(int j = 0; j<n; j++){
            if(propagation[i].label == unique_c[j]){
                tmp = 1;
                break;
            }
        }
        if(tmp == 0){
            unique_c[c] = propagation[i].label;
            c++;
        }
    }

    printf("Graph has been partitioned to %d communitites\n", c);

    for(int i = 0; i<c; i++){
        e = 0;
        printf("Community number: %d\n", i+1);

        for(int j = 0; j<n; j++){
            if(unique_c[i] == propagation[j].label){
                printf("%d ", propagation[j].id);
            }
            if(e==10){
                printf("\n");
            }

        }
        printf("\n");
    }

}

// if draw, returns random node 
int find_most_frequent_value(int *arr, int n){

    int most_frequent;
    int most_frequent_counter = 0;
    int num_canditates;
    int *candidates = malloc(sizeof(int) * n);
    

    for(int i = 0; i<n; i++){

        int counter = 0;

        for(int j =0; j<n; j++){
            if(arr[i]==arr[j]){
                counter++;
            }
        }

        if(counter>most_frequent_counter){
            most_frequent = arr[i];
            most_frequent_counter = counter;
            num_canditates = 1;
            candidates[0]=most_frequent;

        }else if(counter == most_frequent_counter){
            candidates[num_canditates++] = arr[i];
        }
    }

    most_frequent = candidates[rand() % num_canditates];

    return most_frequent;

}


node_propagation *initiaize(Graph *g){

    int n = g->n;

    node_propagation *propagation = malloc(sizeof(node_propagation)* n);
    if(propagation ==NULL){
        fprintf(stderr,"[!] memory allocation failed");
    }

    for(int i = 0; i<n; i++){
        propagation[i].id = i;
        propagation[i].label = i;
        
        propagation[i].nne = malloc(sizeof(int)*g->nodes[i]->ne);
        
        if(propagation[i].nne ==NULL){
            fprintf(stderr,"[!] memory allocation failed");
        }


        for(int k = 0; k<g->nodes[i]->ne; k++){
            propagation[i].nne[k] = g->nodes[i]->links[k]->id;
        }
    }
    printf("initialized\n");
    return propagation;

}

int get_most_frequent_label(int id, Graph *g, node_propagation *propagation){

    int neighbours_count = g->nodes[id]->ne;
    int *neighbours_labels = malloc(sizeof(int)* neighbours_count);
    
    //printf("Node %d neighbors: \n", id);

    for(int i =0; i<neighbours_count; i++){

        int neighbour_id = propagation[id].nne[i];
        int neighbout_label = propagation[neighbour_id].label;
        neighbours_labels[i] = neighbout_label;

        //printf("%d(label:%d) ", propagation[id].nne[i], propagation[propagation[id].nne[i]].label);
    }

    int most_frequent_label = find_most_frequent_value(neighbours_labels, neighbours_count);

    return most_frequent_label;
}

void label_propagation(Graph *g){

    node_propagation *propagation = initiaize(g);

    bool improvement = true;
    int changes = 0;
    int counter = 0;
    int n = g->n;

    int *order = malloc(sizeof(int) * n);
    for(int i = 0; i < n; i++) order[i] = i;

    while(improvement){
        improvement = false;
        changes = 0;

        shuffle(order, n);

        for(int idx = 0; idx<n; idx++){

            int i = order[idx];

            int old_label = propagation[i].label;
            int new_label = get_most_frequent_label(i, g, propagation);
            // printf("new label:%d\n", new_label);

            if(old_label != new_label){
                propagation[i].label = new_label;
                changes++;

                printf("node %d label has changed from %d to %d\n",propagation[i].id, old_label, new_label);
            }
        }
        if(changes != 0){
            improvement = true;
        }
        counter++;
        
    }

    print_communities(propagation, n);

}