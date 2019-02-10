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

graph* create_graph(int);
void set_edge(graph*,int,int,int);
void get_edges(graph*,p_queue*);
p_queue* create_list();
void push(p_queue*,int,int,int);
edge *shift(p_queue*);
size_t size(p_queue*);
void print_list(p_queue*);
void reverse_delete(graph*);
bool is_bridge(graph*,int,int);
void find_bridges(graph*,int,int,int*,int*,p_queue*);

int main(){
	graph *g = create_graph(6);
	set_edge(g,0,1,1);
	set_edge(g,0,3,1);
	set_edge(g,1,2,1);
	set_edge(g,1,3,1);
	set_edge(g,1,4,1);
	set_edge(g,2,3,1);
	set_edge(g,2,4,1);
	set_edge(g,2,5,1);
	set_edge(g,3,5,1);
	reverse_delete(g);
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

void get_edges(graph *g,p_queue *list){
	for(int i = 0;i < g->v_count;i++){
		for(int j = 0;j < i;j++){
			if(g->adj_matrix[i][j]){
				push(list,i,j,g->adj_matrix[i][j]);
			}
		}
	}
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
		if(min_edge->weight < iter->next->weight){
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

size_t size(p_queue *list){
	size_t counter = 0;
	edge *iter = list->head;
	while(iter != NULL){
		counter++;
		iter = iter->next;
	}
	return counter;
}

void print_list(p_queue *list){
	edge *iter = list->head;
	while(iter != NULL){
		printf("%d - %d (%d)\n",iter->vertice_1,iter->vertice_2,iter->weight);
		iter = iter->next;
	}
}

void reverse_delete(graph *g){
	p_queue *list = create_list();
	get_edges(g,list);
	for(int i = 0;i < size(list);i++){
		edge *max_edge = shift(list);
		if(!is_bridge(g,max_edge->vertice_1,max_edge->vertice_2)){
			set_edge(g,max_edge->vertice_1,max_edge->vertice_2,0);
		}
	}
	list = create_list();
	get_edges(g,list);
	print_list(list);
}

bool is_bridge(graph *g,int v1,int v2){
	memset(g->isvisited,0,sizeof(bool) * g->v_count);
	int *ids = malloc(sizeof(int) * g->v_count);
	int *low = malloc(sizeof(int) * g->v_count);
	p_queue *bridges = create_list();
	find_bridges(g,v1,-1,ids,low,bridges);
	edge *iter = bridges->head;
	for(int i = 0;i < size(bridges);i++){
		if((v1 == iter->vertice_1 && v2 == iter->vertice_2) || (v1 == iter->vertice_2 && v2 == iter->vertice_1)){
			return 1;
		}
		iter = iter->next;
	}
	return 0;
}

void find_bridges(graph *g,int at,int parent,int *ids,int *low,p_queue *list){
	static int id = 0;
	ids[at] = low[at] = id++;
	g->isvisited[at] = 1;
	for(int to = 0;to < g->v_count;to++){
		if(g->adj_matrix[at][to]){
			if(to == parent){
				continue;
			}
			if(!g->isvisited[to]){
				find_bridges(g,to,at,ids,low,list);
				low[at] = (low[at] < low[to])? low[at] : low[to];
				if(ids[at] < low[to]){
					push(list,at,to,g->adj_matrix[at][to]);
				}	
			}else{
				low[at] = (low[at] < ids[to])? low[at] : ids[to];
			}
		}
	}
}
