#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct graph graph;
typedef struct segment segment;

struct segment{
	int *vertices;
	size_t v_count;
};

struct graph{
	size_t v_count;
	bool *isvisited;
	int **adj_matrix;
};

graph* create_graph(int);
void set_edge(graph*,int,int,int);
segment* create_segment();
void push_segment(segment*,int);
segment* find_segments(graph*);
void dfs(graph*,int,segment*);
int* find_min(graph*,segment*);
void boruska(graph*);
int count(segment*);

int main(){
	graph *g = create_graph(7);
	set_edge(g,0,1,7);
	set_edge(g,0,5,5);
	set_edge(g,1,2,1);
	set_edge(g,1,6,2);
	set_edge(g,2,3,3);
	set_edge(g,3,4,2);
	set_edge(g,3,6,5);
	set_edge(g,4,5,3);
	set_edge(g,5,6,4);
	segment *s = find_segments(g);
	boruska(g);
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

segment* create_segment(){
	segment *s = malloc(sizeof(segment));
	s->v_count = 0;
	s->vertices = NULL;
	return s;
}

void push_segment(segment *s,int v){
	s->v_count++;
	s->vertices = realloc(s->vertices,sizeof(int) * s->v_count);
	s->vertices[s->v_count - 1] = v;
}

segment* find_segments(graph *g){
	int segment_count = 0;
	segment *segments = NULL;
	for(int i = 0;i < g->v_count;i++){
		if(!g->isvisited[i]){
			segment_count++;
			segments = realloc(segments,sizeof(segment) * segment_count);
			segments[segment_count - 1].v_count = 0;
			segments[segment_count - 1].vertices = NULL;
			dfs(g,i,&segments[segment_count - 1]);
		}
	}
	segment_count ++;
	segments = realloc(segments,sizeof(segment) * segment_count);
	segments[segment_count - 1].v_count = -1;
	segments[segment_count - 1].vertices = NULL;
	memset(g->isvisited,0,sizeof(bool) * g->v_count);

	return segments;
}

void dfs(graph *g,int at,segment *s){
	g->isvisited[at] = 1;
	push_segment(s,at);
	for(int to = 0;to < g->v_count;to++){
		if(g->adj_matrix[at][to] < 0 && !g->isvisited[to]){
			dfs(g,to,s);
		}
	}
}

int* find_min(graph *g,segment *s){
	int *edge = malloc(sizeof(int) * 3);
	edge[0] = -1;
	edge[1] = -1;
	edge[2] = -99;

	for(int i = 0;i < s->v_count;i++){
		g->isvisited[s->vertices[i]] = 1;
	}

	for(int i = 0;i < s->v_count;i++){
		int at = s->vertices[i];
		for(int to = 0;to < g->v_count;to++){
			if(g->adj_matrix[at][to] > 0 && !g->isvisited[to]){
				if(edge[2] == -99 || edge[2] > g->adj_matrix[at][to]){
					edge[0] = at;
					edge[1] = to;
					edge[2] = g->adj_matrix[at][to];
				}
			}
		}
	}
	memset(g->isvisited,0,sizeof(bool) * g->v_count);
	return edge;
}

void boruska(graph *g){
	int edge_count = 0;
	int *edge = NULL;
	int **edges = NULL;
	segment *s = find_segments(g);
	if(count(s) == 1){
		return;
	}
	for(int i = 0;i < count(s);i++){
		edge = find_min(g,&s[i]);
		if(edge[2] != -99){
			printf("(%d - %d), ",edge[0],edge[1]);
			edge_count++;
			edges = realloc(edges,sizeof(int*) * edge_count);
			edges[edge_count - 1] = malloc(sizeof(int) * 3);
			edges[edge_count - 1][0] = edge[0];
			edges[edge_count - 1][1] = edge[1];
			edges[edge_count - 1][2] = edge[2];
		}
	}

	for(int i = 0;i < edge_count;i++){
		g->adj_matrix[edges[i][0]][edges[i][1]] = -edges[i][2];
		g->adj_matrix[edges[i][1]][edges[i][0]] = -edges[i][2];
	}
	printf("\n");
	boruska(g);
}

int count(segment *s){
	segment *iter = s;
	int s_count = 0;
	while(iter->v_count != -1){
		iter++;
		s_count++;
	}
	return s_count;
}
