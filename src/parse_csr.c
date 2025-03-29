#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../lib/parse_csr.h"
#include "../lib/graph.h"

int get_number_of_nodes(char *a_r){

    char* na_r = strdup(a_r);
    char*token = strtok(a_r, ";");
    int num;
    while(token!= NULL){
        num = atoi(token);
        token = strtok(NULL, ";");
    }
    printf("%d\n", num);
    return num;

}

Graph *generate_graph_from_csrrg(char* filename){


    char r[16];
    char positions[1024];
    char a_r[256];
    char edges[4096];
    char edges_offset[512];
    char edges_offset_c[512];
    char edges_offset_g[512];

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

    strcpy(edges_offset_c, edges_offset);
    strcpy(edges_offset_g, edges_offset);

    int rows = atoi(r);

    int c_count = count_numbers(edges_offset_c) -1;
    printf("c_count: %d\n", c_count);
    int *e_offset = tokenize(edges_offset, c_count);

    for(int i = 0; i<c_count; i++){
        printf("%de_off : %d\n", i, e_offset[i]);
    }

    int nodes_count = get_number_of_nodes(a_r);

    Graph *g = graph_init(nodes_count, UNDIRECTED);

    convert(edges, e_offset, g, c_count);

    return g;
}

int *tokenize(char *edges_offset, int c_count){

    char *token;

    int *e_offset = malloc(sizeof(int)*(c_count-1));


    token = strtok(edges_offset, ";");

    for(int i = 0; i<c_count; i++){

        int f = atoi(token);
        token = strtok(NULL, ";");
        int s = atoi(token);
        printf("f: %d s: %d\n", f,s);

        int ammount = s - f; 
        printf("%d\n", ammount);
        e_offset[i] = ammount; 
    }
    return e_offset;

}

void convert(char *edges, int *e_offset, Graph* g, int c_count){
    char *token;
    token = strtok(edges, ";");
    int size = 0;
    int main_node;
    int secondary_node;
    int counter= 0;

    for(int j =0; j<c_count; j++){
        for(int i = 0; i<e_offset[j]-1; i++){
            if(i == 0 && j == 0){
                main_node = atoi(token);
                printf("main node: %d\n", main_node);
            }else if(i == 0){
                main_node = atoi(strtok(NULL, ";"));
                printf("main node: %d\n", main_node);
            }
            token = strtok(NULL, ";");
            printf("%d\n", e_offset[j]-1);
            if(token == NULL)
                continue;
            printf("token: %s\n", token);
            secondary_node = atoi(token);
            size += add_node(g, main_node, secondary_node, size);
            
        }
        printf("\n");
    }

}

int count_numbers(char *edges_offset){

    int counter =1;
    char *token = strtok(edges_offset, ";");

    while(token!=NULL){
        token = strtok(NULL, ";");
        counter ++;
    }
    return counter -1;

}
