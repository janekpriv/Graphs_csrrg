#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"./lib/parse_csr.h"
#include "./lib/graph.h"

int main(int argc, char **argv){

    Graph *g = generate_graph_from_csrrg(argv[1]);

}