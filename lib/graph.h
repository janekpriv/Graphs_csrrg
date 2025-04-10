#ifndef __GRAPH_H__
#define __GRAPH_H__

typedef enum {
    UNDIRECTED,
    DIRECTED
} GraphType;

typedef struct Node {
    int id;
    int ne;
    int comm;
    struct Node **links;
} *Node;

typedef struct Graph {
    GraphType type;
    int n;  
    Node *nodes;
} Graph;

int contains(int, Node*, int);
Node create_Node(int);
void print_list_repr(Graph*);
void free_graph(Graph*);
Graph * graph_init(int, GraphType type);
void link_nodes(Node, Node);
int add_node(Graph*, int, int, int);
void print_adj_matrix_repr(Graph *g);
int cmp(const void *a, const void *b);
void sort_graph(Graph *g);
int edge_count(Graph*);
int edge_exists(Graph*, int, int);
save_list_repr(Graph *g, int parts);
#endif