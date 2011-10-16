#include <stdio.h>
#include <stdlib.h>

#define NETWORK_SIZE 5
#define INITIAL_INFECTED 1

#define DAYS_INFECTIOUS 3
#define DAYS_LATENT 1
#define DAYS_RECOVERED 60

#define R0 2

#define SIMULATION_LENGTH 1865

#include "vertex_node.c"
#include "stack.c"
#include "queue.c"
#include "node_pool.c"
#include "vertex.c"
#include "graph.c"

int main()
{
	graph *g = graph_create();
	
	char file[100];

	int day;

	for(double p = 0.0; p < 0.2; p += 0.01)
	{
		for(int simulation = 0; simulation < 10; simulation++)
		{
			printf("Simulation: %i\n", simulation);

			graph_reset(g);

			graph_circle(g, 2);
			graph_rewire(g, 0.01);
			graph_init_infected(g);

			day = 0;
			while(day < SIMULATION_LENGTH && graph_has_infectious(g) == true)
			{
				//snprintf(file, 100, "graph-data/graph%i.pajek", day);
				//graph_write_pajek(g, &file[0]);

				graph_advance(g, day);

				day++;
			}
		}
	}

	return 0;
}
