#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct graph graph;
typedef struct p_queue p_queue;
typedef struct edge edge;

struct graph{
	size_t v_count;
	bool *isvisited;
	int **adj_matrix;
};

struct p_queue{
	edge *head;
};

struct edge{
	edge *next;
	int vertice_1;
	int vertice_2;
	int weight;
};

graph *create_graph(int);
void set_edge(graph*,int,int,int);
void rotate_edge(graph*,int,int);
p_queue* create_list();
void push(p_queue*,int,int,int);
edge* shift(p_queue*);
void get_edges(graph*,int,p_queue*);
void prim(graph*,int);
bool is_cycle(graph*,int,int);

int main(){
	graph *g = create_graph(7);
	set_edge(g,0,1,5);
	set_edge(g,0,5,5);
	set_edge(g,1,2,1);
	set_edge(g,1,6,2);
	set_edge(g,2,3,3);	
	set_edge(g,3,4,2);
	set_edge(g,3,6,3);
	set_edge(g,4,5,2);
	set_edge(g,5,6,3);
	prim(g,0);	
}

graph* create_graph(int v){
	graph *g = malloc(sizeof(graph));
	g->v_count = v;
	g->isvisited = calloc(v,sizeof(bool));
	g->adj_matrix = malloc(sizeof(int*) * v);
	for(int i = 0;i < v;i++){
		g->adj_matrix[i] = calloc(v,sizeof(int));
	}
	return g;
}

void set_edge(graph *g,int v1,int v2,int w){
	g->adj_matrix[v1][v2] = w;
	g->adj_matrix[v2][v1] = w;
}

void rotate_edge(graph *g,int v1,int v2){
	g->adj_matrix[v1][v2] *= -1;
	g->adj_matrix[v2][v1] *= -1;
}

p_queue* create_list(){
	p_queue *list = malloc(sizeof(p_queue));
	list->head = NULL;
	return list;
}

void push(p_queue *list,int v1,int v2,int w){
	if(list->head == NULL){
		list->head = malloc(sizeof(edge));
		list->head->vertice_1 = v1;
		list->head->vertice_2 = v2;
		list->head->weight = w;
		list->head->next = NULL;
	}else{
		edge *iter = list->head;
		while(iter->next != NULL){
			iter = iter->next;
		}
		iter->next = malloc(sizeof(edge));
		iter->next->vertice_1 = v1;
		iter->next->vertice_2 = v2;
		iter->next->weight = w;
		iter->next->next = NULL;
	}
}

edge* shift(p_queue *list){
	if(list->head == NULL){
		return NULL;
	}
	edge *min_edge = malloc(sizeof(edge));
	min_edge->vertice_1 = list->head->vertice_1;
	min_edge->vertice_2 = list->head->vertice_2;
	min_edge->weight = list->head->weight;
	min_edge->next = NULL;

	edge *iter = list->head;
	edge *ptr = NULL;
	while(iter->next != NULL){
		if(min_edge->weight > iter->next->weight){
			ptr = iter;
			min_edge->vertice_1 = iter->next->vertice_1;
			min_edge->vertice_2 = iter->next->vertice_2;
			min_edge->weight = iter->next->weight;
		}
		iter = iter->next;
	}

	if(ptr == NULL){
		list->head = list->head->next;
	}else{
		ptr->next = ptr->next->next;
	}
	return min_edge;
}

void get_edges(graph *g,int at,p_queue *list){
	g->isvisited[at] = 1;
	for(int to = 0;to < g->v_count;to++){
		if(g->adj_matrix[at][to] > 0){
			push(list,at,to,g->adj_matrix[at][to]);
		}
	}

	for(int to = 0;to < g->v_count;to++){
		if(g->adj_matrix[at][to] < 0 && !g->isvisited[to]){
			get_edges(g,to,list);
		}
	}
}

void prim(graph *g,int at){
	memset(g->isvisited,0,sizeof(bool) * g->v_count);
	p_queue *list = create_list();
	get_edges(g,at,list);
	edge *min_edge = shift(list);
	memset(g->isvisited,0,sizeof(bool) * g->v_count);

	while(min_edge != NULL){
		rotate_edge(g,min_edge->vertice_1,min_edge->vertice_2);
		if(!is_cycle(g,min_edge->vertice_1,-1)){
			break;	
		}else{
			rotate_edge(g,min_edge->vertice_1,min_edge->vertice_2);
			memset(g->isvisited,0,sizeof(bool) * g->v_count);
			min_edge = shift(list);
		}	
	}

	if(min_edge == NULL){
		return;
	}else{
		printf("%d - %d (%d)\n",min_edge->vertice_1,min_edge->vertice_2,min_edge->weight);
		prim(g,at);
	}
}

bool is_cycle(graph *g,int at,int parent){
	g->isvisited[at] = 1;
	for(int to = 0;to < g->v_count;to++){
		if(g->adj_matrix[at][to] < 0){
			if(!g->isvisited[to]){
				if(is_cycle(g,to,at)){
					return 1;
				}
			}else if(to != parent){
				return 1;	
			}
		}
	}
	return 0;
}
