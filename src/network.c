#include <stdio.h>
#include <stdlib.h>

#define NETWORK_SIZE 1000
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
	float T_low, T_high, T_step;
	float R_not_low, R_not_high, R_not_step;

	if(RNOT_SCHEME == 1)
	{
		if(argc != 4)
		{
			printf("Usage: networks R0_low R0_high R0_step\n");
			return 0;
		}

		// Scan in parameters
		sscanf(argv[1], "%f", &R_not_low);
		sscanf(argv[2], "%f", &R_not_high);
		sscanf(argv[3], "%f", &R_not_step);

	}
	else
	{
		if(argc != 4)
		{
			printf("Usage: networks T_low T_high T_step\n");
			return 0;
		}

		// Scan in parameters
		sscanf(argv[1], "%f", &T_low);
		sscanf(argv[2], "%f", &T_high);
		sscanf(argv[3], "%f", &T_step);
	}


	graph *g = graph_create();
	
	char file[100];

	int day;
	double average_times_sick;

	FILE *stats;

	float low, high, step;

	if(RNOT_SCHEME == 1)
	{
		low = R_not_low;
		high = R_not_high;
		step = R_not_step;
	}
	else
	{
		low = T_low;
		high = T_high;
		step = T_step;
	}

	printf("here\n");
	printf("%f %f %f\n", low, high, step);

	for(X = low; X < high; X += step)
	{
		snprintf(file, 100, "data/stats-%f.csv", X);
		printf("%s\n", file);
		stats = graph_open_stats(file);	

		for(int k = 0; k < 40; k++)
		{
			for(double p = 0.0; p < 1; p += 0.01)
			{
				for(int simulation = 0; simulation < 1; simulation++)
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

					average_times_sick = graph_average_times_sick(g);

					graph_write_stats(g, stats, X, p, k, simulation, day, average_times_sick);
					graph_write_stats(g, stats, X, p, k, simulation, day, average_times_sick);
				}

				//printf("%i %f\n", k, p);
			}
		}

		fclose(stats);
	}

	return 0;
}
