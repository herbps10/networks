#ifndef GRAPH_HEADER
#define GRAPH_HEADER

#include "queue.h"
#include "stack.h"
#include "pool.h"

struct graph_struct
{
	vertex_stack *vertices;
	vertex_queue *infectious;
	vertex_queue *latent;
	vertex_node_pool *pool;
};

typedef struct graph_struct graph;

graph *graph_create();
void graph_init(graph *);
void graph_reset(graph *);

void graph_inspect(graph *);

#endif
