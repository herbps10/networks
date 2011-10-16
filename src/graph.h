#ifndef GRAPH_HEADER
#define GRAPH_HEADER

struct graph_struct
{
	struct vertex_struct *vertices[NETWORK_SIZE];

	struct vertex_queue_struct *infectious;
	struct vertex_queue_struct *latent;

	struct vertex_node_pool *pool;
};

typedef struct graph_struct graph;

graph *graph_create();
void graph_init(graph *);
void graph_reset(graph *);

void graph_circle(graph *, int);
void graph_rewire(graph *, double);

void graph_advance(graph *, int);

void graph_inspect(graph *);

#endif
