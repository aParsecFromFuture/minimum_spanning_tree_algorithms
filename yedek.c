#include <stdio.h>
#include <stdlib.h>
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
p_queue* create_list();
void push(p_queue*,int,int,int);
edge* shift(p_queue*);

int main(){

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
	}

	if(ptr == NULL){
		list->head = list->head->next;
	}else{
		ptr->next = ptr->next->next;
	}
	return min_edge;
}
