#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

// CONSTANTS
#define SUSCEPTIBLE	0
#define LATENT 		1
#define INFECTIOUS 	2
#define RECOVERED	3

int state_counter_get(igraph_t *graph, int vertex_index) {
	return igraph_cattribute_VAN(graph, "counter", vertex_index);
}

void state_counter_set(igraph_t *graph, int vertex_index, int value) {
	igraph_cattribute_VAN_set(graph, "counter", vertex_index, value);
}

/**
 * Increments a given state counter array by one
 * 
 * This is used when keeping track of how long an
 * individual has been in each class
 */
void state_counter_increment(igraph_t *graph) {
	for(int i = 0; i < NETWORK_SIZE; i++) {
		 state_counter_set(graph, i, state_counter_get(graph, i) + 1);
	}
}

/**
 * Initializes the given state counter to zero
 * 
 * Used at the beginning of a simulation when
 * all the individuals have been in there current
 * state for zero timesteps
 */
void state_counter_init(igraph_t *graph) {
	for(int i = 0; i < NETWORK_SIZE; i++) {
		state_counter_set(graph, i, 0);
	}
}

/**
 * Convenience function to print out the
 * given state function
 */
void state_counter_print(igraph_t *graph) {
	for(int i = 0; i < NETWORK_SIZE; i++) {
		printf("Vertex %i: %i\n", i, state_counter_get(graph, i));
	}
}

int state_get(igraph_t *graph, int vertex_index) {
	return igraph_cattribute_VAN(graph, "state", vertex_index);
}

void state_set(igraph_t *graph, int vertex_index, int value) {
	igraph_cattribute_VAN_set(graph, "state", vertex_index, value);

	if(value == SUSCEPTIBLE) {
		igraph_cattribute_VAS_set(graph, "color", vertex_index, "white");
	}
	else if(value == LATENT) {
		igraph_cattribute_VAS_set(graph, "color", vertex_index, "gray");
	}
	else if(value == INFECTIOUS) {
		igraph_cattribute_VAS_set(graph, "color", vertex_index, "red");
	}
	else if(value == RECOVERED) {
		igraph_cattribute_VAS_set(graph, "color", vertex_index, "green");
	}
}

/**
 * Initialize the state array to the correct starting configuration
 *
 * Start with setting all verticies to susceptible. Then set
 * a certain number to the infectious class.
 */

void state_init(igraph_t *graph) {
	for(int i = 0; i < NETWORK_SIZE; i++) {
		state_set(graph, i, SUSCEPTIBLE);
	}

	int num_infected = 0;
	while(num_infected < NUM_INFECTED) {
		int random_vertex = rand() % NETWORK_SIZE;
		
		if(state_get(graph, random_vertex) == SUSCEPTIBLE) {
			state_set(graph, random_vertex, INFECTIOUS);
			num_infected++;
		}
	}
}

/**
 * Print out the state of each vertex
 */
void state_print(igraph_t *graph) {
	for(int i = 0; i < NETWORK_SIZE; i++) {
		printf("Vertex %i: ", i);
		
		switch(state_get(graph, i)) {
			case SUSCEPTIBLE:
				printf("S");
				break;
			case LATENT:
				printf("L");
				break;
			case INFECTIOUS:
				printf("I");
				break;
			case RECOVERED:
				printf("R");
				break;
		}

		printf("\n");
	}
}

int *state_counts(igraph_t *graph) {
	static int count[4];

	for(int i = 0; i < 4; i++) {
		count[i] = 0;
	}

	for(int i = 0; i < NETWORK_SIZE; i++) {
		count[state_get(graph, i)] += 1;
	}

	return count;
}

void graph_save(igraph_t *graph, char *name) {
	FILE *file;
	file = fopen(name, "wb");
	igraph_write_graph_graphml(graph, file);
	fclose(file);
}

FILE *stats_open_file() {
	return fopen("stats.csv", "w");
}

void stats_print_header(FILE *file) {
	fprintf(file, "counter,population size,max simulation length,initial infected,rewiring probability,max infected,time of max infected,end susceptible,end latent,end infectious,end recovered,end frame\n");
}

void stats_save(igraph_t *graph, FILE *file, int counter, int K, double p, int max_infected, int time_of_max_infected, int end_susceptible, int end_latent, int end_infectious, int end_recovered, int end_frame) {
	fprintf(file, "%i,%i,%i,%i,%i,%f,%i,%i,%i,%i,%i,%i,%i\n", 
		counter,
		NETWORK_SIZE,
		SIMULATION_LENGTH,
		NUM_INFECTED,
		K,
		p,
		max_infected,
		time_of_max_infected,
		end_susceptible,
		end_latent,
		end_infectious,
		end_recovered,
		end_frame
	);
}

void stats_close_file(FILE *file) {
	fclose(file);
}

void stats_print(igraph_t *graph, FILE *file) {
}

/**
 * Loops through the graph and spreads the infection
 * from sick individuals to their neighbors
 */
void spread_infection(igraph_t *graph) {
	igraph_t new_graph;
	igraph_copy(&new_graph, graph);

	igraph_vector_t neighbors;
	igraph_vector_init(&neighbors, 8);

	/**
	 * Loop through all vertices looking for infected individuals
	 */
	for(int i = 0; i < NETWORK_SIZE; i++) {
		/**
		 * LATENT
		 */
		if(state_get(graph, i) == LATENT) {
			// If it is after its first day in latent state, become infectious
			if(state_counter_get(graph, i) == DAYS_LATENT) {
				state_set(&new_graph, i, INFECTIOUS);
				state_counter_set(&new_graph, i, 0);
			}
		}
		/**
		 * INFECTIOUS
		 */
		else if(state_get(graph, i) == INFECTIOUS) {
			// If it has been infectious for the specified time, move to recovered class
			if(state_counter_get(graph, i) == DAYS_INFECTIOUS) {
				state_set(&new_graph, i, RECOVERED);
				state_counter_set(&new_graph, i, 0);
			}
			else {
				igraph_neighbors(graph, &neighbors, i, IGRAPH_ALL);

				int num_neighbors = igraph_vector_size(&neighbors);
				
				/**
				 * The probability of the disease spreading to a given neighbor
				 * is a function of the infected vertex's degree.
				 * 
				 * The function: T = 1 - (1 - R0/k) * (1/d)
				 * 
				 * Where T is the transmission probability
				 * R0 is a fixed constant representing the R0 of the disease
				 * k is the degree
				 * and d is the number of days spent in the infectious class
				 */

				//double T = 1 - ( 1 - ((double)R0/(double)num_neighbors) ) * (1/(double)DAYS_INFECTIOUS);
				double T = 1 - (1 - ((R0/num_neighbors) / (double)DAYS_INFECTIOUS));

				/**
				 * Loop through the neighbors of the infected individual
				 */
				float random; // Random number to compare against transmission probability T to see if the infection is passed on
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
		/**
		 * RECOVERED
		 */
		else if(state_get(graph, i) == RECOVERED) {
			if(state_counter_get(graph, i) == DAYS_RECOVERED) {
				state_set(&new_graph, i, SUSCEPTIBLE);
				state_counter_set(&new_graph, i, 0);
			}
		}
	}

	state_counter_increment(&new_graph);

	igraph_vector_destroy(&neighbors);

	igraph_copy(graph, &new_graph);
}


int main(void) {
	// Initialize random number seed
	srand(time(NULL));

	igraph_i_set_attribute_table(&igraph_cattribute_table);

	FILE *stats = stats_open_file();
	stats_print_header(stats);

	int counter = 1;
	int *count;
	int max_infected = 0, time_of_max_infected;
	int day;
	for(int K = 1; K < 3; K++) {
		for(double p = 0.0; p < 1.0; p += 0.10) {
			for(int i = 0; i < REPITITIONS; i++) {
				printf("%i\n", counter);
				// Initialize the graph
				igraph_t graph;	
				igraph_watts_strogatz_game(&graph, 1, NETWORK_SIZE, K, p);

				/**
				 * Initialize the variables that keep track of the state of 
				 * each vertex.
				 * 
				 * For each vertex, we track what state it is in (susceptible, latent, infectious, recovered)
				 * and how long they have been in their current state.
				 *
				 */

				state_init(&graph);
				state_counter_init(&graph);

				count = state_counts(&graph);

				/**
				 * The main loop
				 */
				max_infected = 0;
				time_of_max_infected = 1;
				for(day = 1; day <= SIMULATION_LENGTH; day++) {
					spread_infection(&graph);

					char graph_file[100];
					sprintf(&graph_file[0], "graph-data/graph%i-k-%i-p-%f.graphml", day, K, p);

					graph_save(&graph, graph_file);

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

				igraph_destroy(&graph);

				counter += 1;
			}
		}
	}

	stats_close_file(stats);

}
