#ifndef GRAPH_H
#define GRAPH_H
/* graph.h
 * Denzel Murdaugh
 * DenzelM
 * ECE 2230
*/
#include "datatypes.h"
#include "list.h"

//TYPE DEFINITIONS
typedef struct graph_vertex{
	int checked;
	double x;
	double y;
} vtx_t;

typedef struct graph_header{
	int num;
	int** adj_list;
	vtx_t** vtx;
} graph_t;

typedef struct min_path {
	int weight;
	list_t *path;
	int final;	
} dist_t;

//FUNCTION DEFINITIONS
graph_t *graph_construct(int num_vertices);
void graph_destruct(graph_t **G);
void graph_debug(graph_t *G);
void graph_add_edge(graph_t *G, int link_src,int link_weight,int link_dest);
int graph_num_nbrs(graph_t *G, int source);
dist_t *graph_shortest_path(graph_t *G, int src, int dest);
void graph_diameter(graph_t *G, int arr[]);
void graph_free_dij(dist_t *list, int size);
#endif
