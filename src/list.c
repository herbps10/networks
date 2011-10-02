#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define NETWORK_SIZE 20
#define SIMULATION_LENGTH 10
#define INITIAL_INFECTED 5
#define DAYS_INFECTIOUS 3
#define DAYS_RECOVERED 56
#define R0 2.0

enum state { SUSCEPTIBLE, LATENT, INFECTIOUS, RECOVERED };
typedef enum state state;

#include "vertex.h"
#include "vertex_list.h"
#include "graph.h"

#include "vertex_list.c"
#include "graph.c"
#include "vertex.c"

int main()
{	
	graph g = graph_create();

	graph_ring_connect(&g, 1);
	graph_rewire(&g, 0.1);

	graph_init_infected(&g);

	graph_inspect(&g);

	int day = 0;
	while(day < SIMULATION_LENGTH && graph_has_infectious(&g) == 1)
	{
		printf("Day: %i\n", day);

		day = graph_advance(&g, day);

		graph_inspect(&g);
	}

	return 0;
}
