#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../lib/parse_csr.h"
#include "../lib/graph.h"

int get_number_of_nodes(char *a_r){

    int len = strlen(a_r);
    int number_of_nodes = a_r[len-2];
    return number_of_nodes;

}

Graph *generate_graph_from_csrrg(char* filename){


    char r[16];
    char positions[1024];
    char a_r[256];
    char edges[2048];
    char edges_offset[256];


    FILE *in = fopen(filename, "r");
    if(in == NULL){
        fprintf(stderr, "[!] there was a problem with loding a file%s", filename);
        return NULL;
    }

    fgets(r, sizeof(r), in);
    fgets(positions, sizeof(positions), in);
    fgets(a_r, sizeof(a_r), in);
    fgets(edges, sizeof(edges), in);
    fgets(edges_offset, sizeof(edges_offset), in);

    int rows = atoi(r);

    int len = strlen(edges_offset);
    printf("len: %d\n", len);
    int t_count = (len-3)/2; 
    int c_count = t_count;

    int *e_offset = tokenize(edges_offset, c_count);

    int nodes_count = get_number_of_nodes(a_r);

    Graph *g = graph_init(nodes_count, UNDIRECTED);

    convert(edges, e_offset, g);

    return g;
}

int *tokenize(char *edges_offset, int c_count){

    char *token;

    int *e_offset = malloc(sizeof(int)*(c_count));


    token = strtok(edges_offset, ";");

    for(int i = 0; i<c_count; i++){

        int f = atoi(token);
        token = strtok(NULL, ";");
        int s = atoi(token);

        int ammount = s - f; 

        e_offset[i] = ammount; 
    }
    return e_offset;

}

void convert(char *edges, int *e_offset, Graph* g){
    char *token;
    token = strtok(edges, ";");

    int counter = 0;
    int main_node;

    while(token!=NULL){
        for(int i = 0; i<e_offset[counter]; i++){
            if(i == 0 && counter == 0){
                main_node = atoi(token);
            }
            if(i == 0){
                main_node = atoi(strtok(NULL, ";"));
            }
            token = strtok(NULL, ";");
            add_node(g, main_node, atoi(token));
            
        }
        counter++;
    }

}


