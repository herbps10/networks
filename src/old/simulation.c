#include "simulation.h"

/**
 * Performs one simulation
 * Simulation stops when maximum length is reached or no infectious individuals exist
 */
void simulation(FILE *stats, double p, int k, int r)
{
	int day = 0;
	graph g;

	g = graph_create();
	graph_ring_connect(&g, k); // Connect to k neighbors on either side
	graph_rewire(&g, p); // rewire the edges by probability p

	graph_init_infected(&g);
	
	//graph_make_pajek(&g, "graph-data/graph1.pajek");

	// end when SIMULATION_LENGTH reached or graph has no infectious individuals
	while(day < SIMULATION_LENGTH && graph_has_infectious(&g) == 1)
	{
		day = graph_advance(&g, day);

		/*
		snprintf(filename, 100, "graph-data/graph%i.pajek", day);
		graph_make_pajek(&g, filename);
		*/
	}

	// Write one line to stats file
	graph_write_stats(&g, stats, p, k, r, day);

	printf("p = %f, k = %i, r = %i, day = %i\n", p, k, r, day);

	graph_destroy(&g);
}

void test_parameters(FILE *file, simulation_parameters parameters)
{
	double p;
	int k, r;

	for(p = parameters.p_low; p < parameters.p_high; p += parameters.p_step)
	{
		for(k = parameters.k_low; k < parameters.k_high; k += parameters.k_step)
		{
			for(r = 0; r < parameters.repititions; r++)
			{
				simulation(file, p, k, r);
			}
		}
	}
}
