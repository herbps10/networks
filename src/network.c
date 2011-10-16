#include <stdio.h>
#include <stdlib.h>

#define NETWORK_SIZE 100
#define INITIAL_INFECTED 10

#define DAYS_INFECTIOUS 3
#define DAYS_LATENT 1
#define DAYS_RECOVERED 60

#include "vertex_node.c"
#include "stack.c"
#include "queue.c"
#include "node_pool.c"
#include "vertex.c"
#include "graph.c"

int main()
{
	graph *g = graph_create();
	
	graph_circle(g, 2);
	graph_rewire(g, 0.01);

	graph_init_infected(g);

	graph_inspect(g);

	graph_advance(g, 4);

	graph_write_pajek(g, "graph.pajek");

	return 0;
}
