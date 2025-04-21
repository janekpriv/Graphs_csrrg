#ifndef __GRAPH_H__
#define __GRAPH_H__

typedef enum {
    UNDIRECTED,
    DIRECTED
} GraphType;


typedef struct Edge {
    int to;
    int weight;
} Edge;


typedef struct Node {
    int id;
    int ne;
    int comm;
    struct Node **links;
    Edge **edges;
    double dmod;
} *Node;

typedef struct Graph {
    GraphType type;
    int n; // number of nodes
    int e; // number of edges
    Node *nodes;
    int ncomm;
} Graph;


int contains(int, Node*, int);
Node create_Node(int);
void print_list_repr(Graph*);
void free_graph(Graph*);
Graph * graph_init(int, GraphType type);
void link_nodes(Node, Node, int weight);
int add_node(Graph *g, int main_node, int secondary_node, int c, int weight);
void print_adj_matrix_repr(Graph *g);
int cmp_nod(const void *a, const void *b);
void sort_graph(Graph *g);
int edge_count(Graph*);
int edge_exists(Graph*, int, int);
save_list_repr(Graph *g);
#endif