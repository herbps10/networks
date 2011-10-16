#include "graph.h"

graph *graph_create()
{
	graph *g = malloc(sizeof(graph));

	graph_init(g);
	graph_reset(g);

	return g;
}

void graph_init(graph *g)
{
	for(int i = 0; i < NETWORK_SIZE; i++)
	{
		g->vertices[i] = vertex_create();
		g->vertices[i]->id = i;
	}

	g->infectious = vertex_queue_create();
	g->latent = vertex_queue_create();

	g->pool = vertex_node_pool_create();
}

void graph_reset(graph *g)
{
	graph_circle(g, 2);
}

void graph_inspect(graph *g)
{
	for(int i = 0; i < NETWORK_SIZE; i++)
	{
		printf("Vertex: %i\n", g->vertices[i]->id);
	}
}
