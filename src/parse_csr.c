#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../lib/parse_csr.h"
#include "../lib/graph.h"

int get_number_of_nodes(char *a_r){

    char* na_r = strdup(a_r);
    char*token = strtok(na_r, ";");
    int num;
    while(token!= NULL){
        num = atoi(token);
        token = strtok(NULL, ";");
    }
    printf("number of nodes: %d\n", num);
    return num;

}

Graph *generate_graph_from_csrrg(char* filename){
    char *r = NULL;
    char *positions = NULL;
    char *a_r = NULL;
    char *edges_l = NULL;
    char *edges_offset = NULL;

    size_t len = 0;

    FILE *in = fopen(filename, "r");
    if(in == NULL){
        fprintf(stderr, "There was a problem with loading a file %s\n", filename);
        return NULL;
    }

    if (getline(&r, &len, in) == -1 ||
        getline(&positions, &len, in) == -1 ||
        getline(&a_r, &len, in) == -1 ||
        getline(&edges_l, &len, in) == -1 ||
        getline(&edges_offset, &len, in) == -1) {
        
        fprintf(stderr, "Input file is missing required lines or is empty: %s\n", filename);
        fclose(in);
        return NULL;
    }

    fclose(in);
    

    int cols = atoi(r) - 1;  // actual number of columns is n-1

    int e_count = count_numbers(edges_l);
    int id_count = count_numbers(edges_offset);
    int positions_count = count_numbers(positions);
    int in_row_count = count_numbers(a_r);

    int *edges = tokenize(edges_l, e_count);
    int *e_id = tokenize(edges_offset, id_count);
    int *positions_l = tokenize(positions, positions_count);
    int *in_row = tokenize(a_r, in_row_count);

    int nodes_count = get_number_of_nodes(a_r);

    Graph *g = graph_init(nodes_count, UNDIRECTED);

    convert(e_id, edges, e_count, g);
    //matrix_visualisation(cols, in_row_count, positions_l, in_row);

    return g;
}

int *tokenize(char *line, int len){

    char *s = strdup(line);
    char *token;
    int counter = 0;
    int *arr = malloc(sizeof(int)*len);


    token = strtok(s, ";");

    while(token!=NULL){
        arr[counter] = atoi(token);
        token = strtok(NULL, ";");
        counter++;
    }
    return arr;

}


void convert(int *e_id, int *edges, int e_count, Graph *g){

    int c = 0;
    int main_node;
    int secondary;
    int size = 0;

    for(int i = 0; i<e_count; i++){
        if(i == e_id[c]){
            main_node = edges[i];
            c++;
        }else{
            secondary = edges[i];
            size += add_node(g, main_node, secondary, size, 1);
        }
        
    }

}


int count_numbers(char *line){

    char *s = strdup(line);

    int counter =0;
    char *token = strtok(s, ";");

    while(token!=NULL){
        token = strtok(NULL, ";");
        counter ++;
    }
    return counter;
    

}

void matrix_visualisation(int cols, int in_row_count, int *positions, int *in_row){

    FILE *out = fopen("output/matrix.txt", "w");
    if(out == NULL){
        fprintf(stderr, "unable to open the output file, terminating");
    }

    int rows = in_row_count -1;
    int c = 0;
    int o_c = 0;

    for(int i = 0; i<rows; i++){
        int ammount = 0;
        fprintf(out, "[");
        for(int j = 0; j<cols; j++){
            //printf("positions[c] = %d, in_row = %d j == %d\n", positions[c], (in_row[i+1]-in_row[i]), j);
            if(j == positions[c] && ammount < (in_row[i+1]-in_row[i])){
                fprintf(out, "1. ");
                ammount++;
                c++;
                o_c++;
            }else{
                fprintf(out, "0. ");
            }
        }
        fprintf(out, "]\n");
    }
    //printf("only %g percent of a graph are nodes\n", (o_c/(rows*cols))*100);

}
