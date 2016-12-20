/* graph.c
 * Denzel Murdaugh
 * DenzelM
 * ECE 2230
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include "graph.h"
 

dist_t *graph_dijkstra_algorithm(graph_t *G, int src, int dest);
void graph_free_adj(graph_t *G);
void graph_free_vtx(graph_t *G);
int graph_check(graph_t *G);
int graph_find_max_weight(dist_t* DestList, int size, int *pos, int *inf);
int *graph_preperation(int n);

void graph_destruct(graph_t **G){
	graph_free_adj(*G);
	graph_free_vtx(*G);
	free(*G);
	*G=NULL;
}

dist_t *graph_shortest_path(graph_t *G, int src, int dest){
	int i=0;
	int longest=-1;
	int finale;
	dist_t* ShortestDistance=NULL;
	if (dest==-1){
		ShortestDistance = graph_dijkstra_algorithm(G,src,G->num);
		for(i=0;i<G->num;i++){
			if (ShortestDistance[i].weight>longest && ShortestDistance[i].weight!=INT_MAX){
				finale=i;
				longest=ShortestDistance[i].weight;
			}
		}
		ShortestDistance[src].weight=longest;
		ShortestDistance[src].final=finale;
	}
	else{
	ShortestDistance = graph_dijkstra_algorithm(G,src,dest);
	}
	if (ShortestDistance==NULL){
		printf("Massive ERROR\n");
	}
	return ShortestDistance;
}

void graph_diameter(graph_t *G,int arr[]){
	int i,j,n,dist_max = 0,src_pos,dest_pos,inf = 0;
	dist_t *ShortPaths;
	//printf("Working\n");
	for (i = 0; i < G->num; i++)
	{
		ShortPaths = graph_dijkstra_algorithm(G,i,0);
		if(dist_max < (n = graph_find_max_weight(ShortPaths,G->num,&j,&inf))){
			dist_max = n;
			src_pos = i;
			dest_pos = j;
		}
		graph_free_dij(ShortPaths,G->num);
		free(ShortPaths);
	}
	//printf("Working\n");
		if(inf>0)
			printf("Graph is not fully connected.\n");
	arr[0]=dist_max;
	arr[1]=src_pos;
	arr[2]=dest_pos;
	arr[3]=inf;
	//printf("Diameter for graph is: %d (%d,%d)\n", dist_max,src_pos,dest_pos);
}

void graph_add_edge(graph_t* G, int src, int weight, int dest){
	if(weight < 0){
		printf("ERROR: Invalid weight in graph_add_edge() for (%d,%d): %d\n",src,dest,weight);
		return;
	}
	else if(src >= G->num || src < 0){
		printf("ERROR: Invalid src in graph_add_edge(): %d\n", src);
		return;
	}
	else if(dest >= G->num || dest < 0){
		printf("ERROR: Invalid dest in graph_add_edge(): %d\n", dest);
		return;	
	}
	G->adj_list[src][dest] = weight;
	G->adj_list[dest][src] = weight;
}

int graph_num_nbrs(graph_t *G, int source){
	int i=0;
	int count=0;
	for (i=0;i < G->num;i++){
		if(G->adj_list[source][i]!=0 && G->adj_list[source][i]!=INT_MAX){
			count++;
		}
	}
	return count;
}
			
// Initializes graph header elements based on number of vertices.
graph_t *graph_construct(int num_vertices){
	graph_t *G;
	G = (graph_t*) malloc(sizeof(graph_t));
	G->num = num_vertices;
	int i,j = 0;
	vtx_t *vertex;
	G->vtx = (vtx_t**) malloc(sizeof(vtx_t*)*G->num);
	for (i = 0; i < G->num; i++)
	{
		vertex = (vtx_t*) malloc(sizeof(vtx_t));
		vertex->checked = 0;
		vertex->x = -1.0;
		vertex->y = -1.0;		
		G->vtx[i] = vertex;
	}
	G->adj_list = (int**) malloc(sizeof(int*)*G->num);
	for (i = 0; i < G->num; i++)
	{
		G->adj_list[i] = (int*) malloc(sizeof(int)*G->num);
		for (j = 0; j < G->num; j++)
		{
			if(i == j)
				G->adj_list[i][j] = 0;
			else
				G->adj_list[i][j] = INT_MAX;
		}
	}
	return G;
}


dist_t *graph_dijkstra_algorithm(graph_t *G, int src, int dest){
	int count = 1, vertex, MinDistance, MinVtx, i, PrevVertex[G->num];
	dist_t *ShortestDistance;
	ShortestDistance = (dist_t*) malloc(sizeof(dist_t)*G->num);
	ShortestDistance[src].weight = 0;
	ShortestDistance[src].path = list_construct(NULL,NULL);
	while(count <= G->num-1){
		vertex = (src+count)%(G->num);
		ShortestDistance[vertex].weight = G->adj_list[src][vertex];
		ShortestDistance[vertex].path = list_construct(NULL,NULL);
		PrevVertex[vertex] = src;
		count++;
	}
	G->vtx[src]->checked = 1;
	while(!graph_check(G)){
		MinDistance = INT_MAX;
		for (i = 0; i < G->num; i++)
		{
			if(G->vtx[i]->checked == 1)
				continue;
			if(ShortestDistance[i].weight < MinDistance){
				MinDistance = ShortestDistance[i].weight;
				MinVtx = i;
			}
		}
		if(MinDistance == INT_MAX)
			break;
		G->vtx[MinVtx]->checked = 1; 
		list_copy(ShortestDistance[PrevVertex[MinVtx]].path,ShortestDistance[MinVtx].path);
		list_insert(ShortestDistance[MinVtx].path,graph_preperation(MinVtx),LISTPOS_TAIL);
		for (i = 0; i < G->num; i++)
		{
			if(G->vtx[i]->checked == 1 || G->adj_list[MinVtx][i] == INT_MAX)
				continue;
			if(ShortestDistance[MinVtx].weight + G->adj_list[MinVtx][i] < ShortestDistance[i].weight){
				PrevVertex[i] = MinVtx;
				ShortestDistance[i].weight = ShortestDistance[MinVtx].weight + G->adj_list[MinVtx][i];
			}
		}
	}
	for (i = 0; i < G->num; i++)
		G->vtx[i]->checked = 0;
	return ShortestDistance;
}


void graph_free_adj(graph_t *G){
	int i;
	for(i = 0; i < G->num; i++)
		free(G->adj_list[i]);
	free(G->adj_list);
}

void graph_free_vtx(graph_t *G){
	int i;
	for (i = 0; i < G->num; i++)
		free(G->vtx[i]);
	free(G->vtx);
}

void graph_free_dij(dist_t *list, int size){
	int i;
	for (i = 0; i < size; i++)
	{
		list_destruct(list[i].path);
	}
}

int graph_check(graph_t *G){
	int i;
	for (i = 0; i < G->num; i++)
	{
		if(G->vtx[i]->checked == 0)
			return 0;
	}
	return 1;
}

int graph_find_max_weight(dist_t* DestList, int size, int *pos, int *inf){
	int i, max = 0;
	for (i = 0; i < size; i++)
	{
		if(DestList[i].weight == INT_MAX)
			*inf += 1;
		if(DestList[i].weight > max && DestList[i].weight != INT_MAX){
			max = DestList[i].weight;
			*pos = i;
		}
	}
	return max;
}

int *graph_preperation(int n){
	int *i = (int*) malloc(sizeof(int));
	*i = n;
	return i;
}
