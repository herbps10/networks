#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0

#define NETWORK_SIZE 10000
#define SIMULATION_LENGTH 1865
#define INITIAL_INFECTED 5 
#define DAYS_INFECTIOUS 3
#define DAYS_RECOVERED 56
#define R0 2.0

#define NUM_THREADS 4

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

void *thread_main(void *arg)
{
	thread *thread = (struct thread *) arg;
	char filename[100];
	snprintf(filename, 100, "data/stats-%i.csv", thread->index);

	printf("Thread index: %i\n", thread->index);
	
	FILE *stats = graph_open_stats(filename);
	test_parameters(stats, thread->parameters);

	fclose(stats);
}

int main()
{	
	srand(time(NULL));

	simulation_parameters global_parameters = {
		1, 	// K lower bound
		2,	// K upper bound
		1,	// K step

		0.0,	// P lower bound
		0.2, 	// P upper bound
		0.1, 	// P step

		1	// Number of repititions per parameter set
	};

	FILE *stats = graph_open_stats("test.csv");
	test_parameters(stats, global_parameters);

	/*
	int thread_index;
	thread *threads[NUM_THREADS];

	// Boot up all the threads
	for(thread_index = 0; thread_index < NUM_THREADS; thread_index++)
	{
		threads[thread_index] = malloc(sizeof(thread));
		threads[thread_index]->index = thread_index;

		// Every thread does the full range of K values
		threads[thread_index]->parameters.k_low = global_parameters.k_low;
		threads[thread_index]->parameters.k_high = global_parameters.k_high;
		threads[thread_index]->parameters.k_step = global_parameters.k_step;

		// Split up values for P into subranges for each thread
		threads[thread_index]->parameters.p_low = ((global_parameters.p_high - global_parameters.p_low) / NUM_THREADS) * thread_index;
		threads[thread_index]->parameters.p_high = threads[thread_index]->parameters.p_low + ((global_parameters.p_high - global_parameters.p_low) / NUM_THREADS);
		threads[thread_index]->parameters.p_step = global_parameters.p_step;

		// Number of repititions is the same for every thread
		threads[thread_index]->parameters.repititions = global_parameters.repititions;
		
		pthread_create(&threads[thread_index]->thread, NULL, thread_main, threads[thread_index]);
	}

	// Wait until all the threads are done
	for(thread_index = 0; thread_index < NUM_THREADS; thread_index++)
	{
		pthread_join(threads[thread_index]->thread, NULL);

		// Free threads struct
		free(threads[thread_index]);
	}
	*/

	return 0;
}
