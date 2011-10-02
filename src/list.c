#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define NETWORK_SIZE 10000
#define SIMULATION_LENGTH 1865
#define INITIAL_INFECTED 5 
#define DAYS_INFECTIOUS 3
#define DAYS_RECOVERED 56
#define R0 2.0

enum state { SUSCEPTIBLE, LATENT, INFECTIOUS, RECOVERED };
typedef enum state state;

#include "vertex.h"
#include "vertex_list.h"
#include "graph.h"
#include "simulation.h"

#include "vertex_list.c"
#include "graph.c"
#include "vertex.c"
#include "simulation.c"

int main()
{	
	srand(time(NULL));

	test_parameters(
		1, 	// K lower bound
		2,	// K upper bound
		1,	// K step

		0.0,	// P lower bound
		0.2, 	// P upper bound
		0.1, 	// P step

		1	// Number of repititions per parameter set
	);

	return 0;
}
