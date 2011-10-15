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
	g->vertices = vertex_stack_create();

	g->infectious = vertex_queue_create();
	g->latent = vertex_queue_create();

	g->pool = vertex_node_pool_create();
}

void graph_reset(graph *g)
{
	vertex_node *node;
	for(int i = 0; i < NETWORK_SIZE; i++)
	{
		node = vertex_node_pool_get(g->pool);
		node->vertex = i;
		vertex_stack_push(g->vertices, node);
	}
}

void graph_inspect(graph *g)
{
	vertex_node *iterator = g->vertices->head;

	while(iterator != false)
	{
		printf("Vertex: %i\n", iterator->vertex);
		iterator = iterator->next;
	}
}
