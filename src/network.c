#include <stdio.h>
#include <stdlib.h>

#define NETWORK_SIZE 10

#include "vertex_node.c"
#include "stack.c"
#include "queue.c"
#include "graph.c"
#include "node_pool.c"
#include "vertex.c"

int main()
{
	graph *g = graph_create();

	graph_inspect(g);

	return 0;
}
