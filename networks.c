#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <igraph.h>

// CONFIGURATION
#define NETWORK_SIZE 10
#define SIMULATION_LENGTH 10 //1825
#define NUM_INFECTED 5

#define R0 2.0

#define DAYS_LATENT 		1
#define DAYS_INFECTIOUS 	3
#define DAYS_RECOVERED  	56

// CONSTANTS
#define SUSCEPTIBLE	0
#define LATENT 		1
#define INFECTIOUS 	2
#define RECOVERED	3

int state[NETWORK_SIZE]; // Stores the state of each vertex
int state_counter[NETWORK_SIZE]; // Stores how long each vertex has been in its current class

/**
 * Increments a given state counter array by one
 * 
 * This is used when keeping track of how long an
 * individual has been in each class
 */
void state_counter_increment(int *state_counter) {
	for(int i = 0; i < NETWORK_SIZE; i++) {
		 state_counter[i] += 1;
	}
}

/**
 * Initializes the given state counter to zero
 * 
 * Used at the beginning of a simulation when
 * all the individuals have been in there current
 * state for zero timesteps
 */
void state_counter_init(int *state_counter) {
	for(int i = 0; i < NETWORK_SIZE; i++) {
		state_counter[i] = 0;
	}
}

/**
 * Convenience function to print out the
 * given state function
 */
void state_counter_print(int *state_counter) {
	for(int i = 0; i < NETWORK_SIZE; i++) {
		printf("Vertex %i: %i\n", i, state_counter[i]);
	}
}

/**
 * Initialize the state array to the correct starting configuration
 *
 * Start with setting all verticies to susceptible. Then set
 * a certain number to the infectious class.
 */

void state_init(int *state) {
	for(int i = 0; i < NETWORK_SIZE; i++) {
		state[i] = SUSCEPTIBLE;
	}

	int num_infected = 0;
	while(num_infected < NUM_INFECTED) {
		int random_vertex = rand() % NETWORK_SIZE;
		
		if(state[random_vertex] == SUSCEPTIBLE) {
			state[random_vertex] = INFECTIOUS;
			num_infected++;
		}
	}
}

/**
 * Print out the state of each vertex
 */
void print_state(int *state) {
	for(int i = 0; i < NETWORK_SIZE; i++) {
		printf("Vertex %i: ", i);
		
		switch(state[i]) {
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
		if(state[i] == INFECTIOUS) {
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
			double T = 1 - ((1 - ((double)R0/num_neighbors)) / ((double)DAYS_INFECTIOUS));

			printf("Vertex %i has %i neighbors and a transmission rate of %f\n", i, num_neighbors, T);

			/**
			 * Loop through the neighbors of the infected individual
			 */
			for(int n = 0; n < num_neighbors; n++) {
				
			}
		}
	}

	igraph_vector_destroy(&neighbors);

	igraph_copy(graph, &new_graph);
}


int main(void) {
	// Initialize random number seed
	srand(time(NULL));

	igraph_t graph;	

	/**
	 * Initialize the variables that keep track of the state of 
	 * each vertex.
	 * 
	 * For each vertex, we track what state it is in (susceptible, latent, infectious, recovered)
	 * and how long they have been in their current state.
	 *
	 */

	state_init(state);
	state_counter_init(state_counter);

	// Initialize the graph
	igraph_watts_strogatz_game(&graph, 1, NETWORK_SIZE, 1, 0.5);
	//igraph_erdos_renyi_game(&graph, IGRAPH_ERDOS_RENYI_GNM, 100, 0.5, 0, 0);
	printf("%i\n", (int)igraph_vcount(&graph));

	/**
	 * The main loop
	 */
	for(int day = 1; day <= SIMULATION_LENGTH; day++) {
		spread_infection(&graph);
		printf("%i\n", day);
	}

	igraph_destroy(&graph);
}
