#include <stdio.h>
#include <stdlib.h>

#define NETWORK_SIZE 1000
#define INITIAL_INFECTED 5

#define DAYS_INFECTIOUS 3
#define DAYS_LATENT 1
#define DAYS_RECOVERED 60

#define R0 2

#define SIMULATION_LENGTH 1865

double T = 0.001;

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

	FILE *stats;

	for(T = 0.00; T < 0.05; T += 0.005)
	{
		snprintf(file, 100, "data/stats-%i.csv", (int)(T * 10000));
		printf("%s\n", file);
		stats = graph_open_stats(file);	

		for(int k = 0; k < 40; k++)
		{
			for(double p = 0.0; p < 1; p += 0.01)
			{
				for(int simulation = 0; simulation < 10; simulation++)
				{
					graph_reset(g);

					graph_circle(g, k);
					graph_rewire(g, p);
					graph_init_infected(g);

					day = 0;
					while(day < SIMULATION_LENGTH && graph_has_infectious(g) == true)
					{
						//snprintf(file, 100, "graph-data/graph%i.pajek", day);
						//graph_write_pajek(g, &file[0]);

						graph_advance(g, day);

						day++;
					}

					graph_write_stats(g, stats, T, p, k, simulation, day);
				}

				//printf("%i %f\n", k, p);
			}
		}

		fclose(stats);
	}

	return 0;
}
