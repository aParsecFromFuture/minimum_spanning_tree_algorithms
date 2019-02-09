#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct graph graph;

struct graph{
	size_t v_count;
	int **adj_matrix;
	bool *isvisited;
};

graph* create_graph(int);
void set_edge(graph*,int,int,int);
int** get_edges(graph*);
void find_bridges(graph*,int,int,int*,int*,int**);
bool is_bridge(graph*,int,int);
bool is_cycle(graph*,int,int);
int** create_edges();
int** push(int**,int,int,int);
int min(int,int);
void bubble_sort(int**);
void print_biridges(int**);
void kruskal(graph*);
int degree(graph*,int);

int main(){
	graph *g = create_graph(7);
	set_edge(g,0,1,5);
	set_edge(g,0,5,5);
	set_edge(g,1,2,1);
	set_edge(g,1,6,2);
	set_edge(g,2,3,3);
	set_edge(g,3,6,3);
	set_edge(g,3,4,2);
	set_edge(g,4,5,2);
	set_edge(g,5,6,3);
	kruskal(g);
}

graph* create_graph(int v){
	graph* g = malloc(sizeof(graph));
	g->v_count = v;

	g->isvisited = malloc(sizeof(bool) * v);
	memset(g->isvisited,0,sizeof(bool) * v);

	g->adj_matrix = malloc(sizeof(int*) * v);
	for(int i = 0;i < v;i++){
		g->adj_matrix[i] = malloc(sizeof(int) * v);
		memset(g->adj_matrix[i],0,sizeof(int) * v);
	}

	return g;
}

void set_edge(graph *g,int v1,int v2,int w){
	g->adj_matrix[v1][v2] = w;
	g->adj_matrix[v2][v1] = w;
}

int** get_edges(graph *g){
	int **edges = create_edges();
	for(int i = 0;i < g->v_count;i++){
		for(int j = 0;j < i;j++){
			if(g->adj_matrix[i][j]){
				edges = push(edges,i,j,g->adj_matrix[i][j]);
			}
		}
	}
	return edges;
}

void find_bridges(graph *g,int at,int parent,int *ids,int *low,int **bridges){
	static int id = 0;
	ids[at] = low[at] = id;
	id++;

	g->isvisited[at] = 1;
	for(int to = 0;to < g->v_count;to++){
		if(g->adj_matrix[at][to] > 0){
			if(to == parent){
				continue;
			}
			if(!g->isvisited[to]){
				find_bridges(g,to,at,ids,low,bridges);
				low[at] = min(low[at],low[to]);
				if(ids[at] < low[to]){
					push(bridges,at,to,g->adj_matrix[at][to]);
				}
			}else{
				low[at] = min(low[at],ids[to]);
			}
		}
	}
}

bool is_bridge(graph *g,int v1,int v2){
	int *ids = malloc(sizeof(int) * g->v_count);
	int *low = malloc(sizeof(int) * g->v_count);
	int **bridges = create_edges();
	find_bridges(g,v1,-1,ids,low,bridges);
	memset(g->isvisited,0,sizeof(bool) * g->v_count);
	for(int i = 0;i < bridges[0][0];i++){
		if(v1 == bridges[i + 1][0] || v1 == bridges[i + 1][1]){
			if(v2 == bridges[i + 1][0] || v2 == bridges[i + 1][1]){
				return true;
			}
		}
	}
	return false;
}

bool is_cycle(graph *g,int at,int parent){
	g->isvisited[at] = 1;
	for(int to = 0;to < g->v_count;to++){
		if(g->adj_matrix[at][to]){
			if(!g->isvisited[to]){
				if(is_cycle(g,to,at)){
					return true;
				}
			}else if(to != parent){
				return true;
			}
		}
	}
	return false;
}

int** create_edges(){
	int **edges = malloc(sizeof(int*));
	edges[0] = malloc(sizeof(int) * 3);
	memset(edges[0],0,sizeof(int) * 3);
	return edges;
}

int** push(int **edges,int v1,int v2,int w){
	int b_count = edges[0][0] + 1;
	edges = realloc(edges,sizeof(int*) * (b_count + 1));
	edges[b_count] = malloc(sizeof(int) * 3);
	edges[b_count][0] = v1;
	edges[b_count][1] = v2;
	edges[b_count][2] = w;
	edges[0][0]++;
	return edges;
}

int min(int n1,int n2){
	return (n1 < n2)? n1 : n2;
}

void bubble_sort(int **bridges){
	int b_count = bridges[0][0];
	for(int i = 1;i < b_count;i++){
		for(int j = 1;j < b_count - i + 1;j++){
			if(bridges[j][2] > bridges[j + 1][2]){
				int swap[3];
				swap[0] = bridges[j][0];
				swap[1] = bridges[j][1];
				swap[2] = bridges[j][2];

				bridges[j][0] = bridges[j + 1][0];
				bridges[j][1] = bridges[j + 1][1];
				bridges[j][2] = bridges[j + 1][2];

				bridges[j + 1][0] = swap[0];
				bridges[j + 1][1] = swap[1];
				bridges[j + 1][2] = swap[2];
			}
		}
	}
}

void print_bridges(int **bridges){
	int b_count = bridges[0][0];
	for(int i = 1;i <= b_count;i++){
		printf("%d - %d (%d)\n",bridges[i][0],bridges[i][1],bridges[i][2]);
	}
}

void kruskal(graph *g){
	int** edges = get_edges(g);
	bubble_sort(edges);
	for(int i = 0;i < g->v_count;i++){
		memset(g->adj_matrix[i],0,sizeof(int) * g->v_count);
	}
	for(int i = 0;i < edges[0][0];i++){
		int v1 = edges[i + 1][0];
		int v2 = edges[i + 1][1];
		int w = edges[i + 1][2];
		set_edge(g,v1,v2,w);
		if(is_cycle(g,v1,-1)){
			set_edge(g,v1,v2,0);
		}else{
			printf("%d - %d (%d)\n",v1,v2,w);
		}
		memset(g->isvisited,0,sizeof(bool) * g->v_count);
	}
}

int degree(graph *g,int v1){
	int d_count = 0;
	for(int i = 0;i < g->v_count;i++){
		if(g->adj_matrix[v1][i]){
			d_count++;
		}
	}
	return d_count;
}
