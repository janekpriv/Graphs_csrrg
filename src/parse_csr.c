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
        fprintf(stderr, "[!] there was a problem with loding a file%s", filename);
        return NULL;
    }

    getline(&r, &len, in);
    getline(&positions, &len, in);
    getline(&a_r, &len, in);
    getline(&edges_l, &len, in);
    getline(&edges_offset, &len, in);


    int rows = atoi(r);

    int e_count = count_numbers(edges_l);
    int id_count = count_numbers(edges_offset);

    int *edges = tokenize(edges_l, e_count);
    int *e_id = tokenize(edges_offset, id_count);
    int nodes_count = get_number_of_nodes(a_r);

    Graph *g = graph_init(nodes_count, UNDIRECTED);

    convert(e_id, edges, e_count, g);

    return g;
}

int *tokenize(char *s, int len){

    char *token;
    int counter = 0;
    int *arr = malloc(sizeof(int)*len);


    token = strtok(s, ";");

    while(token!=NULL){
        arr[counter] = atoi(token);
        token = strtok(NULL, ";");
        printf("%d\n",arr[counter]);
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
            size += add_node(g, main_node, secondary, size);
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
    printf("counter: %d\n", counter);
    return counter;
    

}
