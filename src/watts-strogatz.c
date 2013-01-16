/**
 * Goal of this code:
 *
 * Look at R0 over different values of p and k
 * by looking at how many infections are caused by infecting one individual at a time in a network
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NETWORK_SIZE 100
#define INITIAL_INFECTED 5

#define R0 2
#define RNOT_SCHEME 1
#define SIMULATION_LENGTH 1865
#define WRITE_BUFFER_SIZE 5000

#define REPS 100

#define DAYS_INFECTIOUS 3
#define DAYS_LATENT 1
#define DAYS_RECOVERED 10000

double X;
int k;

#include "vertex_node.c"
#include "stack.c"
#include "queue.c"
#include "node_pool.c"
#include "vertex.c"
#include "graph.c"

int main(int argc, char *argv[])
{
	graph *g = graph_create();

	int day;
	vertex_node *infected;

	double rnot_sum = 0;
	double local_rnot_sum = 0;
	double rnot_average = 0;
	
	FILE *fp = fopen("/home/herb/git/networks/data/watts-myway.csv", "w");
	fprintf(fp, "simulation, p, k, R0, infected\n");

	X = 2;

	for(k = 2; k < 60; k++)
	{
		printf("%i\n", k);
		//X = 1.0 - pow(1.0 - 2.0/(double)k, 1.0 / (double)DAYS_INFECTIOUS);

		for(double p = 0; p <= 1; p += 0.1)
		{
			for(int v = 0; v < NETWORK_SIZE; v++)
			{
				local_rnot_sum = 0;

				for(int simulation = 0; simulation < REPS; simulation++)
				{
					graph_reset(g);

					graph_circle( g, k);
					graph_rewire(g, p);
							
					infected = vertex_node_pool_get(g->pool);
					infected->vertex = g->vertices[v];
					vertex_queue_enqueue(g->infectious, infected);
					vertex_set_state(g->vertices[v], INFECTIOUS, 0);

					day = 0; 
					while(day < SIMULATION_LENGTH && graph_has_infectious(g) == true)
					{
						graph_advance(g, day);

						day++;
					}

					fprintf(fp, "%i, %f, %i, %i, %f\n", simulation, p, k, g->vertices[v]->infections_caused, graph_max_infected(g));
				}
			}

		}
	}

	fclose(fp);
}
