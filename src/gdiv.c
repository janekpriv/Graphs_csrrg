#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "../lib/graph.h"

int count_numbers(char *l){
    char tmp[strlen(l)+1];
    strcpy(tmp, l);
    int counter = 0;
    char *token = strtok(tmp, ";");

    while(token!=NULL){
        token = strtok(NULL, ";");
        counter ++;
    }
    return counter;

}

int *convert_to_arr(char *l, int n){

    int *arr = malloc(sizeof(int)*n);
    if (arr == NULL)
        return NULL;
    char *token = strtok(l, ";");

    int i = 0;
    while(token!=NULL && i < n){
        arr[i++] = atoi(token);

        token = strtok(NULL, ";");
    }
    return arr;

}

void print_arr(int *arr, int arr_size){
    for (int i = 0; i < arr_size; i++){
        printf("%d;", arr[i]);
    }
    printf("\n");
}






int main (int argc, char **argv) {
    FILE *in = argc == 2 ? fopen(argv[1], "r") : NULL;
    if(in == NULL){
        fprintf(stderr, "[!] there was a problem with loading a file %s\n", argv[1]);
        return 1;
    }

    char *l1 = NULL, *l2 = NULL, *l3 = NULL, *l4 = NULL, *l5 = NULL;
    size_t len = 0;
    
    // Read lines dynamically
    if (getline(&l1, &len, in) == -1 ||
        getline(&l2, &len, in) == -1 ||
        getline(&l3, &len, in) == -1 ||
        getline(&l4, &len, in) == -1 ||
        getline(&l5, &len, in) == -1) {
        perror("Error reading lines");
        fclose(in);
        free(l1); free(l2); free(l3); free(l4); free(l5);
        return 1;
    }

    FILE * gr_info = fopen("./output/graph_info.txt", "w");
    if (gr_info == NULL){
        fprintf(stderr, "[!] there was a problem with loading a file \"graph_info\"\n");
        return 1;
    }
    
    int cols; 
    assert(sscanf(l1, "%d", &cols) == 1);
    fprintf(gr_info, "cols = %d\n", cols);
    printf("cols = %d\n", cols);
    

    int nodes = count_numbers(l2);
    assert(nodes > 0);
    fprintf(gr_info, "nodes = %d\n", nodes);
    printf("nodes = %d\n", nodes);
    Graph *g = graph_init(nodes, UNDIRECTED);


    int rows = count_numbers(l3);
    assert(rows > 0);
    fprintf(gr_info, "rows = %d\n", rows);
    printf("rows = %d\n", rows);
    

    int edg_size = count_numbers(l4); 
    // edges array
    int * edges = convert_to_arr(l4, edg_size);
    assert(edges != NULL);
    //print_arr(edges, edg_size);

    int  ind_size = count_numbers(l5);
    // main nodes indices array
    int * indices = convert_to_arr(l5, ind_size);
    assert(indices != NULL);
    //print_arr(indices, ind_size);

    FILE *out = fopen("./output/output.txt", "w");

    int n_edg = 0; // number of edges


    int size = 0;
    int main_node, secondary_node;
    for (int i = 0 , j = 0, 
        main_in = indices[j];
         i < edg_size; i++){
        
        if (i == main_in){
            main_node = edges[i];
            
            main_in = indices[++j];
            //printf("m = %d\n", main_node);
        }
        else {
            secondary_node = edges[i];
            //printf("s = %d\n", secondary_node);
            size += add_node(g, main_node, secondary_node, size);
            fprintf(out,"connect %d to %d\n", main_node, secondary_node);
            n_edg++;
        }

    }

    fprintf(gr_info, "edges: %d\n", n_edg);
    printf("edges: %d\n", n_edg);
    fclose(gr_info);
    sort_graph(g);
    print_adj_matrix_repr(g);
    print_list_repr(g);

    free(l1);
    free(l2);
    free(l3);
    free(l4);
    free(l5);
    
}