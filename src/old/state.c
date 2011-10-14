enum state_enum {SUSCEPTIBLE, LATENT, INFECTIOUS, RECOVERED};
typedef enum state_enum state;

struct graph_state_struct {
	state condition[NETWORK_SIZE];
	int counter[NETWORK_SIZE];
} graph_state, new_graph_state;

/***********************************
 ********* STATE COUNTER ***********
 ***********************************/

/**
 * Get the state day counter for a vertex
 */
int state_counter_get(, int vertex_index) {
	return graph_state.counter[vertex_index];
	//return igraph_cattribute_VAN(graph, "counter", vertex_index);
}

/**
 * Set the day counter on a given vertex in a graph
 */
void state_counter_set(igraph_t *graph, int vertex_index, int value) {
	graph_state.counter[vertex_index] = value;
	//igraph_cattribute_VAN_set(graph, "counter", vertex_index, value);
}
/**
 * Increments a graph state counter array by one
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
 * Initializes the graph state counter to zero
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
 * graph state counter
 */
void state_counter_print(igraph_t *graph) {
	for(int i = 0; i < NETWORK_SIZE; i++) {
		printf("Vertex %i: %i\n", i, state_counter_get(graph, i));
	}
}

/***********************************
 ************ STATE ****************
 ***********************************/

/**
 * Get a vertex's state
 */
int state_get(igraph_t *graph, int vertex_index) {
	return graph_state.condition[vertex_index];
	//return igraph_cattribute_VAN(graph, "state", vertex_index);
}

/**
 * Set a vertex state
 * Also sets the color attribute for use in R when plotting the graph
 */
void state_set(igraph_t *graph, int vertex_index, int value) {
	graph_state.condition[vertex_index] = value;
	/*
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
	*/
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
	int random_vertex;
	while(num_infected < NUM_INFECTED) {
		random_vertex = rand() % NETWORK_SIZE;
		
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

/**
 * Count the number of individuals in each state
 * and return values as array
 */
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


