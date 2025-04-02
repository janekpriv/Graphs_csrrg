#include <stdlib.h>
#include <stdio.h>
#include "../lib/graph.h"
#undef DEBUG
int contains(int id, Node *list, int size) {
    for (int i = 0; i < size; i++) {
        if (list[i]->id == id) {
            return i;
        }
    }
    return -1;
}

Node create_Node(int id){
    Node v = malloc(sizeof(struct Node));
    if (!v) return NULL;
    v->id = id;
    v->ne = 0;
    v->links = NULL;
    return v;
}



Graph *graph_init(int n, GraphType type) {
    Graph *g = malloc(sizeof(Graph));
    if (g == NULL) return NULL;

    g->type = type;
    g->n = n;

    g->nodes = malloc(n * sizeof(Node*));
    if (g->nodes == NULL) {
        free(g);
        return NULL;
    }
    
    return g;
}

void link_nodes(Node node1, Node node2) {
    /* node1 -> node2*/
    node1->links[node1->ne] = node2;
    node1->ne++;
}

void print_list_repr(Graph *g){

    FILE *out = fopen("output/graph.txt", "w");
    if(out == NULL){
        fprintf(stderr, "unable to open the output file, terminating");
    }

    if (g == NULL) {
        printf("Graph is NULL\n");
        return;
    }
    printf("Adjacency List Representation:\n");
    for (int i = 0; i < g->n; i++) {
        printf("%d: ", g->nodes[i]->id);
        fprintf(out,"%d: ", g->nodes[i]->id);
        for (int j = 0; j < g->nodes[i]->ne; j++) {
            printf("%d ", g->nodes[i]->links[j]->id);
            fprintf(out, "%d ", g->nodes[i]->links[j]->id);
    
        }
        printf("\n");
        fprintf(out,"\n");
    }
    fclose(out);
}


void free_graph(Graph *g) {
    if (!g) return;

    for (int i = 0; i < g->n; i++) {
        if (g->nodes[i] != NULL) {
            if (g->nodes[i]->links != NULL) {
                free(g->nodes[i]->links);
            }
            free(g->nodes[i]);
        }
    }
    if (g->nodes != NULL) {
        free(g->nodes);
    }
    free(g);
}

int add_node(Graph *g, int main_node, int secondary_node, int c){
    
    int i;
    int added_nodes = 0;
    Node node_1, node_2;
    if ((i = contains(main_node, g->nodes, c)) == -1){
        node_1 = create_Node(main_node);
        g->nodes[c] = node_1;
        g->nodes[c++]->links =malloc(((g->n)-1)* sizeof(struct Node *));
        added_nodes++;
    }else{
        node_1 = g->nodes[i];
    }
    if ((i = contains(secondary_node, g->nodes, c)) == -1){
        node_2 = create_Node(secondary_node);
        g->nodes[c] =node_2;
        g->nodes[c++]->links =malloc(((g->n)-1)* sizeof(struct Node *));
        added_nodes++;
    }else{
        node_2 = g->nodes[i];
    }
    if(node_1 && node_2){
        link_nodes(node_1, node_2);
        link_nodes(node_2, node_1);
       // printf("adding %d - %d\n", main_node, secondary_node);
    }   
    return added_nodes;

}

int edge_count(Graph*g){

    int edges = 0;

    for(int i = 0; i<g->n; i++){
        if(g->nodes[i] != NULL){
            edges +=g->nodes[i]->ne;
        }
    }

    return edges/2; //undirected graph

}

int edge_exists(Graph *g, int index1, int index2) {
    
    Node node1 = g->nodes[index1];
    if (!node1) return 0; 

    for (int i = 0; i < node1->ne; i++) {
        if (node1->links[i]->id == index2) {
            return 1; 
        }
    }
    
    return 0; 
}