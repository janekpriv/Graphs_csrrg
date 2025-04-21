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
    v->comm = -1;
    v->links = NULL;
    v->dmod = 0.0;
    v->edges = NULL;
    return v;
}



Graph *graph_init(int n, GraphType type) {
    Graph *g = malloc(sizeof(Graph));
    if (g == NULL) return NULL;

    g->type = type;
    g->n = n;
    g->e = 0;
    g->ncomm = 0;
    g->nodes = malloc(n * sizeof(Node*));
    if (g->nodes == NULL) {
        free(g);
        return NULL;
    }
    
    return g;
}

void link_nodes(Node node1, Node node2, int weight) {
    /* node1 -> node2*/
    for (int i = 0; i < node1->ne; i++) {
        if (node1->links[i] == node2) {
            node1->edges[i]->weight += weight;
            //printf(" %d - %d exists\n", node1->id, node2->id);
            return;
        }
        
    }
    node1->links[node1->ne] = node2;
    
    Edge *edg = malloc(sizeof(struct Edge));
    edg->to = node1->links[node1->ne]->id;
    edg->weight = weight;
    node1->edges[node1->ne] = edg;
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
    //printf("Adjacency List Representation:\n");
    for (int i = 0; i < g->n; i++) {
        //printf("%d: ", g->nodes[i]->id);
        fprintf(out,"%d: ", g->nodes[i]->id);
        for (int j = 0; j < g->nodes[i]->ne; j++) {
            //printf("%d ", g->nodes[i]->links[j]->id);
            fprintf(out, "%d ", g->nodes[i]->links[j]->id);
    
        }
        //printf("\n");
        fprintf(out,"\n");
    }
    fclose(out);
}

save_list_repr(Graph *g){
    int PARTS = 3;
    char folder[30];
    sprintf(folder, "pg%dx%d", g->n, g->e);
    char save_output_command[100];
    sprintf(save_output_command, "mkdir -p ./output/%s", folder);
    system(save_output_command);

    if (g == NULL) {
        printf("Graph is NULL\n");
        return;
    }

    for (int k = 0; k < PARTS; k++){
        char filename[64];
        sprintf(filename, "./output/%s/graph%d", folder, k );
        FILE *out = fopen(filename, "w");
        if(out == NULL){
            fprintf(stderr, "unable to open the output file, terminating");
        }

        for (int i = 0; i < g->n; i++) {
            if (g->nodes[i]->comm == k){
                fprintf(out,"%d: ", g->nodes[i]->id);
            for (int j = 0; j < g->nodes[i]->ne; j++) {
                Node neighbor = g->nodes[i]->links[j];
                if (neighbor->comm == k)
                    fprintf(out, "%d ", g->nodes[i]->links[j]->id);
        
            }
            fprintf(out,"\n");
                
            }
            
        }
        fclose(out);
    }
    
}


void print_adj_matrix_repr(Graph *g){
    FILE *out = fopen("output/adjm.txt", "w");
    FILE *bout = fopen("output/adjm.bin","wb");
    if(out == NULL){
        fprintf(stderr, "unable to open the output file, terminating");
    }

    if (g == NULL) {
        printf("Graph is NULL\n");
        return;
    }
    
    // nodes and links MUST be sorted for it to work (sort_graph handles that)
    for (int i = 0; i < g->n; i++){
        int k = 0;
        //fprintf(out, "[");
        int ne = g->nodes[i]->ne;
        for (int j = 0; j < g->n; j++){
            if (k < ne && j == g->nodes[i]->links[k]->id){
                fprintf(out, "1 ");

                // Writing to the binary file (1 byte for 1)
                char value = 1;
                fwrite(&value, sizeof(char), 1, bout);

                k++;
                
            }
            else {
                fprintf(out, "0 ");

                 // Writing to the binary file (1 byte for 0)
                 char value = 0;
                 fwrite(&value, sizeof(char), 1, bout);
            }
        }
        fprintf(out, "\n");
        //fprintf(out, "]\n");
    }
    fclose(out); 
    fclose(bout);  
}


int cmp_nod(const void *a, const void *b) {
    return ((*(struct Node**)a)->id) - ((*(struct Node**)b)->id);
}

int cmp_edg(const void *a, const void *b) {
    return ((*(struct Edge**)a)->to) - ((*(struct Edge**)b)->to);
}

// cleaner  but longer version of cmp
/*
int cmp(const void *a, const void *b){
    const Node nodeA = *((const Node*)a);
    const Node nodeB = *((const Node*)b);
    return nodeA->id - nodeB->id;
}
*/
void sort_graph(Graph *g){
    qsort(g->nodes, g->n, sizeof(Node), cmp_nod);
    
    for (int i = 0; i < g->n; i++){
        int ne = g->nodes[i]->ne;
        if (ne > 0) {
            qsort(g->nodes[i]->links, ne, sizeof(Node), cmp_nod);
            if (g->nodes[i]->edges != NULL)
                qsort(g->nodes[i]->edges, ne, sizeof(Edge), cmp_edg);
        }
    }

    
}

void free_graph(Graph *g) {
    if (!g) return;

    for (int i = 0; i < g->n; i++) {
        if (g->nodes[i] != NULL) {
            if (g->nodes[i]->links != NULL) {
                free(g->nodes[i]->links);
            }
            if (g->nodes[i]->edges != NULL) {
                for (int j = 0; j < g->nodes[i]->ne; j++)
                    free(g->nodes[i]->edges[j]);
                free(g->nodes[i]->edges);
            }
            free(g->nodes[i]);
        }
    }
    if (g->nodes != NULL) {
        free(g->nodes);
    }
    free(g);
}

int add_node(Graph *g, int main_node, int secondary_node, int c, int weight){
    
    int i;
    int added_nodes = 0;
    Node node_1, node_2;
    if ((i = contains(main_node, g->nodes, c)) == -1){
        node_1 = create_Node(main_node);
        g->nodes[c] = node_1;
        g->nodes[c]->links =malloc((g->n)* sizeof(struct Node *));
        g->nodes[c]->edges = malloc((g->n)*sizeof(struct Edge*));
        c++;
        added_nodes++;
    }else{
        node_1 = g->nodes[i];
    }
    if ((i = contains(secondary_node, g->nodes, c)) == -1){
        node_2 = create_Node(secondary_node);
        g->nodes[c] =node_2;
        g->nodes[c]->links =malloc((g->n)* sizeof(struct Node *));
        g->nodes[c]->edges = malloc((g->n)*sizeof(struct Edge*));
        c++;
        added_nodes++;
    }else{
        node_2 = g->nodes[i];
    }

    
    link_nodes(node_1, node_2, weight);
    g->e++; // increment total number of edges in a graph
    if(node_1 && node_2){
        link_nodes(node_2, node_1, weight);
        
    }
    //printf("adding %d - %d\n", main_node, secondary_node);
   
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