#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>

#include "../lib/louvian.h"
#include "../lib/graph.h"

#define DEBUG 

double get_modularity(int *communities, Graph *g){

    int n = g->n;
    int m  = edge_count(g); 
    int denom = 2*m - 1;

    double quality = 0;
    int k1, k2, A;
    for(int i = 0; i<n; i++){
        for(int j = i+1; j<n; j++){
            // check if node is in the same community 
            if(g->nodes[i]->comm == g->nodes[j]->comm){

                k1 = g->nodes[i]->ne;
                k2 = g->nodes[j]->ne;

                A = edge_exists(g, i, j);

                quality += (double)A - (double)(k1*k2)/(double)(denom);
            }


        }
    }
    return  quality/((double)m); // don't divide by 2 because we didn't compute duplicates

} 


double delta_modularity(int community, Node u, Graph *g){

    int n = g->n;
    int number_of_edges  = edge_count(g);

    // m: total sum of edge weights in the graph (constant during one pass)
    int m = edge_count(g);

    // ku: weighted degree of vertex u (sum of weights of all edges incident to u)
    int ku = u->ne;

    // ku→c: sum of weights of edges between vertex u and vertices in community c
    int ku_to_c = 0;
    for (int i = 0; i < ku; i++){
        if (u->links[i]->comm == community)
        ku_to_c++;
    }

    // Σc^: sum of weighted degrees of all vertices currently in community c
    int Sigma_c_hat = 0;
    for (int i = 0; i < g->n; i++){
        if (g->nodes[i]->comm == community)
        Sigma_c_hat++;
    }

    // Calculate delta Q (proportional value, omitting 1/m factor for efficiency)
    double delta_Q = (double)ku_to_c - (double)(Sigma_c_hat * ku) / (double)(2.0 * m);
    
    return delta_Q;
}



int count_communities(Graph* g){
    int * unique_c = malloc(sizeof(int)* g->n);
    int c = 0;

    for(int i = 0; i<g->n; i++){
        int tmp = 0 ;
        for(int j = 0; j<c; j++){
            if(g->nodes[i]->comm == unique_c[j]){
                tmp = 1;
                break;
            }
        }
        if(tmp == 0){
            unique_c[c] = g->nodes[i]->comm;
            c++;
        }
    }
    free(unique_c);
    return c;

}

#include <time.h>
double time1, timedif;


void print_communities(Graph * g){
    FILE *file = fopen("./output/communities", "w");

    int c = count_communities(g);
    int assigned_nodes = 0;
    printf("Total communities: %d\n", c);
    if(file)
        fprintf(file, "Total communities: %d\n", c);
    for (int i = 0; i < c; i++){
        if (file)
            fprintf(file, "Community %d: ", i);
        int comm_size = 0;
        for (int j = 0; j < g->n; j++){
            if (g->nodes[j]->comm == i){
                if (file)
                    fprintf(file," %d ", j);
                comm_size++;
                assigned_nodes++;
            }
        }
        if (file)
            fprintf(file,"\n");
    }
    if (file)
    fprintf(file, "Total assigned nodes: %d out of %d\n\n", assigned_nodes, g->n);
    fclose(file);
}

// Find pair of communities to merge (simplest: smallest sizes)
void find_merge_pair(Graph *g, int *comm1, int *comm2) {
    int *comm_sizes = calloc(count_communities(g) + 1, sizeof(int));
    int max_comm = 0;

    // Count sizes and find max community ID
    for (int i = 0; i < g->n; i++) {
        int comm = g->nodes[i]->comm;
        comm_sizes[comm]++;
        if (comm > max_comm) max_comm = comm;
    }
    //printf("max comm = %d\n", max_comm);

    // Find two smallest communities
    int smallest1 = -1, smallest2 = -1;
    int size1 = g->n + 1, size2 = g->n + 1;
    for (int c = 0; c <= max_comm; c++) {
        if (comm_sizes[c] > 0) {
            if (comm_sizes[c] < size1) {
                size2 = size1;
                smallest2 = smallest1;
                size1 = comm_sizes[c];
                smallest1 = c;
            } else if (comm_sizes[c] < size2) {
                size2 = comm_sizes[c];
                smallest2 = c;
            }
        }
    }
    
    *comm1 = smallest1;
    *comm2 = smallest2;
    free(comm_sizes);
}

// Find largest community by node count
int find_largest_community(Graph *g) {
    int *comm_sizes = calloc(count_communities(g), sizeof(int));
    int max_comm = 0;

    for (int i = 0; i < g->n; i++) {
        int comm = g->nodes[i]->comm;
        comm_sizes[comm]++;
        if (comm > max_comm) max_comm = comm;
    }

    int largest_comm = 0, max_size = 0;
    for (int c = 0; c <= max_comm; c++) {
        if (comm_sizes[c] > max_size) {
            max_size = comm_sizes[c];
            largest_comm = c;
        }
    }

    free(comm_sizes);
    return largest_comm;
}


void relocate_solitary_nodes(Graph *g, int comm_num){
    for (int i = 0; i < g->n; i++) {
        Node node = g->nodes[i];
        for (int j = 0; j < comm_num; j++){
            bool found = false;
            if (node->comm == j){
                // check if the node has neighbors (atleast one) that share the same community
                for (int k = 0; k < node->ne; k++){
                    if (node->links[k]->comm == j){
                        found = true;
                        break;
                    }
                }
                if (!found)
                    node->comm = node->links[0]->comm; 
                else if (found)
                    break;
            }
        }
    }
}

// Split a community (simple: split into two by re-running Phase 1 on subgraph)
void split_community(Graph *g, int comm_to_split, int curr_comm_count, int desired_k) {
    // Extract subgraph for the community
    int *nodes_in_comm = malloc(g->n * sizeof(int));
    int node_count = 0;
    for (int i = 0; i < g->n; i++) {
        if (g->nodes[i]->comm == comm_to_split) {
            nodes_in_comm[node_count++] = i;
        }
    }
    printf("subgraph extracted\n");

    // Create a temporary subgraph
    Graph *subgraph = graph_init(node_count, UNDIRECTED);

     
    for (int i = 0; i < node_count; i++) {
        subgraph->nodes[i] = create_Node(i); // Initialize all nodes
    } 
    int c = 0; 
    for (int i = 0; i < node_count; i++) {
        int node_i = nodes_in_comm[i];
        for (int j = 0; j < g->nodes[node_i]->ne; j++) {
            int neighbor = g->nodes[node_i]->links[j]->id;
            //printf("%d -> ", i);
            
            for (int k = 0; k < node_count; k++) {
                
                if (nodes_in_comm[k] == neighbor) {
                    
                    c += add_node(subgraph, i,k, c); // Add edge in subgraph
                    //printf("%d ", k);
         
                    break;
                }
            }
            //printf("\n");
            
            
        }
        //printf("\n");
    }
    printf("subgraph filled with values: assigned %d of %d nodes\n", c, node_count);
    //sort_graph(subgraph);
    int target_splits = desired_k - curr_comm_count + 1;
    // Run Phase 1 on subgraph to split into 2+ communities
    phase1(subgraph, subgraph, target_splits);
    printf("phase 1 finished for the subgraph\n");
    // Map back to original graph with new community IDs
    int new_comm_id = curr_comm_count; // Start with next available ID
    for (int i = 0; i < node_count; i++) {
        int orig_node = nodes_in_comm[i];
        int sub_comm = subgraph->nodes[i]->comm;
        if (sub_comm == 0) {
            g->nodes[orig_node]->comm = comm_to_split; // Keep original ID for one part
        } else {
            g->nodes[orig_node]->comm = new_comm_id + sub_comm - 1; // New IDs for others
        }
    }

    free_graph(subgraph);
    free(nodes_in_comm);
}

void remap(Graph *g){
printf("start remapping\n");
    int communities_count = count_communities(g);
    
    int *new_communities = malloc(sizeof(int)*communities_count);
    for (int i = 0; i < communities_count; i++) new_communities[i] = -1;
    
    int k = 0;
    for(int i = 0; i<g->n && k <communities_count; i++){
        int found = 0;
        for (int j = 0; j <k; j++){
            if (new_communities[j] == g->nodes[i]->comm){
                found = 1;
                break;
            }
        }
        if (!found)
        new_communities[k++] = g->nodes[i]->comm;
    }

   
    for(int i = 0; i < g->n ; i++){
        for (int j = 0; j <communities_count; j++){
            if (new_communities[j] == g->nodes[i]->comm){
                g->nodes[i]->comm = j;
                break;
            }
        }
    }

}



void louvain(Graph *g, int desired_k){
    bool improvement = true;
    Graph *current_g = g;
    double modularity = 0.0;
    int comm_count, new_comm_count;

    while (improvement) {
        improvement = false;

    #ifdef DEBUG
        puts("Starting phase 1");
    #endif
        comm_count = current_g->n;
        phase1(current_g, g, desired_k); // pass original graph (g) to update comm
        //comm_count = count_communities(current_g);
        
        //if (comm_count <= desired_k) break;

    #ifdef DEBUG
        puts("Starting phase 2");
    #endif
        Graph *cg = phase2(current_g); // communities graph
        //if (!cg) break;
    
        if ((new_comm_count = (count_communities(g))) < comm_count && new_comm_count > desired_k) { 
    #ifdef DEBUG
        printf("Community count decreased: %d -> %d\n", comm_count, new_comm_count);
    #endif
            improvement = true;
            if (current_g != g) free_graph(current_g); // free intermediate graph
            current_g = cg;
        } else {
    #ifdef DEBUG
            printf("Community count %d -> %d\n", comm_count, new_comm_count);
    #endif
            free_graph(cg); // no change, discard cg
            break;
        }
    }
    printf("COMM COUNT INITIALLY = %d", comm_count);
    relocate_solitary_nodes(g, comm_count);
    while (comm_count != desired_k) {
        if (comm_count > desired_k) {
            // Merge communities
            int comm1, comm2;
            find_merge_pair(g, &comm1, &comm2); // Find pair to merge
            change_communities(g, comm1, comm2); // Merge comm1 into comm2
            comm_count--;
            printf("Merged %d into %d, new count: %d\n", comm1, comm2, comm_count);
            remap(g);
        } else if (comm_count < desired_k) {
            // Split a community
            int comm_to_split = find_largest_community(g);
            printf("Largest comm found: %d\n", comm_to_split);
            split_community(g, comm_to_split, comm_count, desired_k);
            comm_count = count_communities(g); // Recount after split
            printf("Split community %d, new count: %d\n", comm_to_split, comm_count);
        }
    }

    // Final output
    print_communities(g);
    printf("Final modularity: %lf\n", get_modularity(NULL, g)); // Pass NULL since communities array not used

    
    if (current_g != g) free_graph(current_g);
}

//change old community to new one
void change_communities(Graph *g, int old_comm, int new_comm) {
    for (int i = 0; i < g->n; i++) {
        if (g->nodes[i]->comm == old_comm) {
            g->nodes[i]->comm = new_comm;
        }
    }
}

void seed_communities(Graph *g) {
    for (int i = 0; i < g->n; i++)
        g->nodes[i]->comm = i;
}


// phase 1 of Louvain algorithm: local optimization
#include <math.h>

void phase1(Graph *g, Graph *og, int target_comm_count){
    int n = g->n;
    int *communities = malloc( sizeof(int) * n);
    int curr_community;

    time1 = (double) clock(); 
    seed_communities(g);
    time1 = time1 / CLOCKS_PER_SEC;
    int iteration = 0;
    
    bool improvement = true;
    int max_iterations = 4;
    double threshold = 1e-4; 
    
    //double best_modularity = -1;
    while(improvement && iteration < max_iterations){
        improvement = false;
        iteration++;
        if (iteration%100 == 0)
            printf("Phase1 iteration %d\n", iteration);
        for(int i = 0; i<n; i++){
            double best_delta_Q = 0.0;
            int best_community = g->nodes[i]->comm;
            for(int j = 0; j<g->nodes[i]->ne; j++){
                    curr_community = g->nodes[i]->comm;
                    int neighbor_comm = g->nodes[i]->links[j]->comm;
                    if (neighbor_comm == curr_community)
                        continue; // no point in trying same community
                    //double modularity = get_modularity(communities, g);
                    double delta_Q = delta_modularity(neighbor_comm, g->nodes[i], g);
                    //printf("modularity: %lf  best modularity %lf\n", modularity, best_modularity);
                    if(delta_Q>best_delta_Q){
                        
                        best_delta_Q = delta_Q;
                        best_community = neighbor_comm;
                        
                    }
                    
            }
            if (best_delta_Q > 0.0){
                //printf("delta_Q  %lf- \n", best_delta_Q);
                g->nodes[i]->comm = best_community;
                improvement = true;
            }
            
            
            
        }
        //if (count_communities(g) <= target_comm_count) break; // Stop at target
        //printf("Finished Phase1 iteration %d\n", iteration);
        
    }
    printf("Finished optimization loop\n");

    int communities_count = count_communities(g);
    

    
    int *new_communities = malloc(sizeof(int)*communities_count);
    for (int i = 0; i < communities_count; i++) new_communities[i] = -1;
    
    int k = 0;
    for(int i = 0; i<n && k <communities_count; i++){
        int found = 0;
        for (int j = 0; j <k; j++){
            if (new_communities[j] == g->nodes[i]->comm){
                found = 1;
                break;
            }
        }
        if (!found)
        new_communities[k++] = g->nodes[i]->comm;
    }

    printf("Counted communities\n");
    for(int i = 0; i < og->n ; i++){
        bool changed = false;
        //printf("node %d community %d becomes\n", i, og->nodes[i]->comm);
        for (int j = 0; j <communities_count; j++){
            if (new_communities[j] == og->nodes[i]->comm){
                
                og->nodes[i]->comm = j;
                changed = true;
                //printf(" part of community %d\n", j);
                break;
            }
        }
        if (!changed){
            og->nodes[i]->comm = g->nodes[og->nodes[i]->comm]->comm;
            //printf("changed the community to %d\n", g->nodes[og->nodes[i]->comm]->comm);
            i--;
        }
        
    }
    printf("Changed og values to new communities\n");

    if (og != g){
        for(int i = 0; i < g->n ; i++){
            for (int j = 0; j <communities_count; j++){
                if (new_communities[j] == g->nodes[i]->comm){
                    g->nodes[i]->comm = j;
                    break;
                }
            }
        }
    }

    timedif = ( ((double) clock()) / CLOCKS_PER_SEC) - time1;
    printf("The elapsed time is %f seconds\n", timedif);

    /*
    double delta_mod = fabs(new_Q - prev_Q);

    if (delta_mod < threshold) {
        printf("ΔQ below threshold (%e). Stopping.\n", threshold);
        break;
    }

    prev_Q = new_Q;
    */
    free(new_communities);
    free(communities);
}


// phase 2 of Louvain algorithm: aggregation
Graph *phase2(Graph *g){
    int n = g->n;
    int communities_count = count_communities(g);
    if (communities_count <= 1) return NULL;
    // graph where every node is a community
    Graph * cg = graph_init(communities_count, UNDIRECTED);

    int c = 0; 
    for(int i = 0; i<n ; i++){
        int comm1 = g->nodes[i]->comm;
        for (int j = 0; j< g->nodes[i]->ne; j++){
            int comm2 = g->nodes[i]->links[j]->comm;
            if (comm1 != comm2 ){
                    c += add_node(cg,comm1, comm2, c);
                    //printf("adding %d - %d\n", comm1, comm2);
                
            }

        }
        
    }
    sort_graph(cg);
    
    return cg;

}
