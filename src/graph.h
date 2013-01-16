#ifndef GRAPH_HEADER
#define GRAPH_HEADER

#include "node_pool.h"

struct graph_struct
{
	struct vertex_struct *vertices[NETWORK_SIZE];

	struct vertex_queue_struct *infectious;
	struct vertex_queue_struct *latent;

	struct vertex_node_pool_struct *pool;
};

typedef struct graph_struct graph;

graph *graph_create();
void graph_init(graph *);
void graph_reset(graph *);

void graph_circle(graph *, int);
void graph_rewire(graph *, double);

void graph_advance(graph *, int);

void graph_inspect(graph *);

void graph_reset(graph *);

_Bool graph_has_infectious(graph *);

void graph_write_stats(graph *, FILE *, double, double, int, int, int, double, double, int);

FILE *graph_open_stats(char *);

double graph_average_times_sick(graph *);

double graph_max_infected(graph *g);

#endif
