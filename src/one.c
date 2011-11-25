#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NETWORK_SIZE 10000
#define INITIAL_INFECTED 5

#define DAYS_INFECTIOUS 3
#define DAYS_LATENT 1
#define DAYS_RECOVERED 60

#define R0 2

#define RNOT_SCHEME 1

#define SIMULATION_LENGTH 1865

double T = 0.001;
double R_not = 1;
double X;

#include "vertex_node.c"
#include "stack.c"
#include "queue.c"
#include "node_pool.c"
#include "vertex.c"
#include "graph.c"

int main(int argc, char *argv[])
{
	graph *g = graph_create();

	graph_circle(g, 40);

	graph_rewire(g, 0.5);

	graph_write_pajek(g, "test.pajek");
}
