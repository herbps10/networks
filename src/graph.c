/**
 * Returns a new, initialized graph
 */
graph graph_create()
{
	graph g;
	graph_init(&g);

	return g;
}

/**
 * Initialize fields of graph struct
 */
void graph_init(graph *g)
{
	g->infectious = NULL;
	g->latent = NULL;

	graph_vertices_create(g);
}

/**
 * Frees all memory related to the graph
 */
void graph_destroy(graph *g)
{
	int i;
	for(i = 0; i < NETWORK_SIZE; i++)
	{
		vertex_destroy(g->vertices[i]);
	}

	vertex_list_destroy_vertices(g->infectious);
	vertex_list_destroy_vertices(g->latent);

	vertex_list_destroy(g->infectious);
	vertex_list_destroy(g->latent);
}

/**
 * Fills the vertices array with vertices
 *
 * No edges are created in between the vertices
 */
void graph_vertices_create(graph *g)
{
	int i;
	for(i = 0; i < NETWORK_SIZE; i++)
	{
		g->vertices[i] = malloc(sizeof(vertex));
		*g->vertices[i] = vertex_create();
	}
}

/**
 * Connect the vertices in a graph into a ring structure
 *
 * Takes each vertex and connects it to its nearest
 * K neighbors on either side of the ring
 * (where the nearest neighbors are the ones nearest
 * in the vertices array)
 */
void graph_ring_connect(graph *g, int K)
{
	int i, k, before, after;
	for(i = 0; i < NETWORK_SIZE; i++)
	{
		for(k = 1; k <= K; k++)
		{
			before = i - k; // The vertex to connect to on the left of the current vertex (smaller index)
			after = i + k; // The vertex to connect to on the right of the current vertex (larger index)

			if(before < 0) before += NETWORK_SIZE; // If we're less than 0 wrap around to the other end
			if(after >= NETWORK_SIZE) after -= NETWORK_SIZE; // If we've gone past the end of the array wrap around to the other end

			vertex_add_edge(g->vertices[i], g->vertices[before]);
			vertex_add_edge(g->vertices[i], g->vertices[after]);
		}
	}
}

/**
 * Performs Watts-Strogatz style rewiring of the graph
 *
 * Looks at each vertex in the graph and randomly rewires
 * one of its edges by probability p
 */

void graph_rewire(graph *g, double p)
{
	int i, random_vertex;
	float random;
	vertex_list_node *iterator;
	
	for(i = 0; i < NETWORK_SIZE; i++)
	{
		// Generate a random number between 0 and 1
		// if value is less than p, then rewire the edge
		random = (double)rand() / RAND_MAX;

		iterator = g->vertices[i]->first_neighbor;

		while(iterator != NULL)
		{
			if(random < p)
			{
				// Keep generating random vertex indices until we find one that we aren't connected to
				do
				{
					// Generate a random vertex index
					random_vertex = ((double)rand() / (double)RAND_MAX) * NETWORK_SIZE;
				} while(random_vertex != i && vertex_edge_exists(g->vertices[i], g->vertices[random_vertex]) == FALSE);

				vertex_delete_edge(g->vertices[i], iterator->vertex);
				vertex_add_edge(g->vertices[i], g->vertices[random_vertex]);
			}

			iterator = iterator->next;
		}
	}
}

/**
 * Randomly set a certain number of vertices as infected
 */
void graph_init_infected(graph *g)
{
	int infected = 0, r;

	do
	{
		r = ((double)rand() / (double)RAND_MAX) * NETWORK_SIZE;
		
		// Check to make sure the vertex isn't already infectious
		if(g->vertices[r]->state != INFECTIOUS)
		{
			// This is always going to be used at the start of a new simulation,
			// so days will be 0
			vertex_set_state(g->vertices[r], INFECTIOUS, 0);

			// Add this individual to the graph's list of infected individuals
			g->infectious = vertex_list_append(g->infectious, g->vertices[r]);

			infected++;
		}
	}
	while(infected < INITIAL_INFECTED);
}

/**
 * Advances the graph simulation by one day
 *
 * Loops through each vertex and increments counter
 * Infectious individuals have a chance of spreading to infection
 *
 * Returns the new day number (day + 1)
 */
int graph_advance(graph *g, int day)
{
	vertex_list_node *infectious_iterator = g->infectious, *latent_iterator = g->latent, *new_latent, *temp;

	// Loop through the list of infectious vertices in the graph
	while(infectious_iterator != NULL)
	{
		// Check to see if the invididual is ready to move to the recovered class.
		// If so, set its state to recovered, and remove it from the list of
		// infectious individuals.
		if(day - infectious_iterator->vertex->state_day > DAYS_INFECTIOUS)
		{
			// Set state to recovered
			vertex_set_state(infectious_iterator->vertex, RECOVERED, day);
			if(infectious_iterator->prev == NULL)
			{
				g->infectious = infectious_iterator->next;
				if(infectious_iterator->next != NULL)
				{
					g->infectious->prev = NULL;
				}
				
				temp = infectious_iterator;
				infectious_iterator = infectious_iterator->next;
				free(temp);
			}
			else
			{
				infectious_iterator->prev->next = infectious_iterator->next;
				temp = infectious_iterator;
				free(temp);
				infectious_iterator = infectious_iterator->next;
				free(temp);
			}
		}
		else
		{
			// Have the vertex try to infect its neighbors. It will return a list of 
			// neighbors that it succesfully infected (new_infectious).
			new_latent = vertex_infect_neighbors(infectious_iterator->vertex, day);

			// if anyone was infected, append the list to the graph's list of
			// latent people.
			if(new_latent != NULL)
			{
				g->latent = vertex_list_merge(g->latent, new_latent);
			}

			
			// Move to the next infectious individual
			infectious_iterator = infectious_iterator->next;
		}
	}

	// FIXME:
	// I'm hardcoding a one day latent period
	// This is easier to code because we can assume
	// that any vertices in the latent list can be moved
	// immediately to infectious
	if(latent_iterator != NULL)
	{

		while(latent_iterator != NULL)
		{
			vertex_set_state(latent_iterator->vertex, INFECTIOUS, day);

			latent_iterator = latent_iterator->next;
		}

		g->infectious = vertex_list_merge(g->infectious, g->latent);

		g->latent = NULL;
	}

	return day + 1;
}

/**
 * Prints out the state of each vertex
 */
void graph_inspect(graph *g)
{
	int i;
	for(i = 0; i < NETWORK_SIZE; i++)
	{
		printf("Vertex %i:\t", i);

		switch(g->vertices[i]->state)
		{
			case SUSCEPTIBLE:
				printf("S\n");
				break;
			case LATENT:
				printf("L\n");
				break;
			case INFECTIOUS:
				printf("I\n");
				break;
			case RECOVERED:
				printf("R\n");
				break;
		}
	}
}

/**
 * Checks to see if the graph has infectious individuals
 */
_Bool graph_has_infectious(graph *g)
{
	if(g->infectious == NULL) return FALSE;
	return TRUE;
}
