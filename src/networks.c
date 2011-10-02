#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include <igraph.h>

// CONFIGURATION
#define NETWORK_SIZE 1000
#define SIMULATION_LENGTH 1825
#define NUM_INFECTED 10
#define REPITITIONS 1

#define R0 2.0

#define DAYS_LATENT 		1
#define DAYS_INFECTIOUS 	3
#define DAYS_RECOVERED  	56

#define NUM_THREADS 4

#include "state.c"
#include "stats.c"

struct parameters_struct {
	int K_low;
	int K_high;
	int K_step;

	double p_low;
	double p_high;
	double p_step;
};

typedef struct parameters_struct parameters;

void graph_save(igraph_t *graph, char *name) {
	FILE *file;
	file = fopen(name, "wb");
	igraph_write_graph_graphml(graph, file);
	fclose(file);
}

/**
 * Loops through the graph and spreads the infection
 * from sick individuals to their neighbors
 */
void spread_infection(igraph_t *graph, igraph_t *new_graph) {
	igraph_vector_t neighbors;

	double T;
	float random; // Random number to compare against transmission probability T to see if the infection is passed on
	int num_neighbors;

	// Loop through all vertices looking for infected individuals
	for(int i = 0; i < NETWORK_SIZE; i++) {
		// LATENT
		if(state_get(graph, i) == LATENT) {
			// If it is after its first day in latent state, become infectious
			if(state_counter_get(graph, i) == DAYS_LATENT) {
				state_set(&new_graph, i, INFECTIOUS);
				state_counter_set(&new_graph, i, 0);
			}
		}
		// INFECTIOUS
		else if(state_get(graph, i) == INFECTIOUS) {
			// If it has been infectious for the specified time, move to recovered class
			if(state_counter_get(graph, i) == DAYS_INFECTIOUS) {
				state_set(&new_graph, i, RECOVERED);
				state_counter_set(&new_graph, i, 0);
			}
			else {
				igraph_neighbors(graph, &neighbors, i, IGRAPH_ALL);
				num_neighbors = igraph_vector_size(&neighbors);
				
				 // The probability of the disease spreading to a given neighbor
				 // is a function of the infected vertex's degree.
				 //
				 // The function: T = 1 - (1 - R0/k) * (1/d)
				 //
				 // Where T is the transmission probability
				 // R0 is a fixed constant representing the R0 of the disease
				 // k is the degree
				 // and d is the number of days spent in the infectious class

				T = 1 - (1 - ((R0/num_neighbors) / (double)DAYS_INFECTIOUS));

				// Loop through the neighbors of the infected individual
				
				for(int n = 0; n < num_neighbors; n++) {

					// Only infect the neighbor if they are susceptible
					if(state_get(graph, (int)VECTOR(neighbors)[n]) == SUSCEPTIBLE) {

						// Generates float from 0.0-1.0 inclusive
						random = (float)rand()/(float)RAND_MAX; 

						// Pass on the transmission with probability T
						if(random < T) {
							state_set(&new_graph, (int)VECTOR(neighbors)[n], LATENT);
							state_counter_set(&new_graph, (int)VECTOR(neighbors)[n], 0);
						}

					}
				}

			}
		}
		// RECOVERED
		else if(state_get(graph, i) == RECOVERED) {
			if(state_counter_get(graph, i) == DAYS_RECOVERED) {
				state_set(&new_graph, i, SUSCEPTIBLE);
				state_counter_set(&new_graph, i, 0);
			}
		}
	}

	igraph_vector_destroy(&neighbors);

	state_counter_increment(&new_graph);

	igraph_copy(graph, &new_graph);

	igraph_destroy(&new_graph);
}

int run_iterations(int run_index, parameters params) {
	FILE *stats = stats_open_file(run_index);
	stats_print_header(stats);

	int counter = 1;
	int *count;
	int max_infected = 0, time_of_max_infected;
	int day;
	
	igraph_t graph;
	
	for(int K = params.K_low; K < params.K_high; K += params.K_step) {
		for(double p = params.p_low; p < params.p_high; p += params.p_step) {
			for(int i = 0; i < REPITITIONS; i++) {
				printf("%i\n", counter);

				// Initialize the graph
				igraph_watts_strogatz_game(&graph, 1, NETWORK_SIZE, K, p);

				//
				// Initialize the variables that keep track of the state of 
				// each vertex.
				 
				// For each vertex, we track what state it is in (susceptible, latent, infectious, recovered)
				// and how long they have been in their current state.

				state_init(&graph);
				state_counter_init(&graph);

				count = state_counts(&graph);

				//
				// The main loop
				//
				max_infected = 0;
				time_of_max_infected = 1;
				for(day = 1; day <= SIMULATION_LENGTH; day++) {
					spread_infection(&graph);

					//char graph_file[100];
					//sprintf(&graph_file[0], "graph-data/graph%i-k-%i-p-%f.graphml", day, K, p);

					//graph_save(&graph, graph_file);

					count = state_counts(&graph);

					if(count[INFECTIOUS] > max_infected) {
						max_infected = count[INFECTIOUS];
						time_of_max_infected = day;
					}

					if(count[INFECTIOUS] == 0) {
						break;
					}
				}

				stats_save(&graph, stats, counter, K, p, max_infected, time_of_max_infected, count[SUSCEPTIBLE], count[LATENT], count[INFECTIOUS], count[RECOVERED], day);

				counter += 1;
				igraph_destroy(&graph);
			}
		}
	}
	

	stats_close_file(stats);
}

int main(void) {
	// Initialize random number seed
	srand(time(NULL));

	//igraph_i_set_attribute_table(&igraph_cattribute_table);

	parameters p = {
		1, 5, 1,
		0.0, 0.2, 0.01
	};

	run_iterations(0, p);
}
