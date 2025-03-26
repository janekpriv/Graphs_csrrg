#ifndef __GRAPH_H__
#define __GRAPH_H__

typedef enum {
    UNDIRECTED,
    DIRECTED
} GraphType;

typedef struct Node {
    int id;
    int ne;
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
void add_node(Graph*, int, int);
#endif