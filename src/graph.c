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

	g->infectious_tail = NULL;
	g->latent_tail = NULL;

	graph_vertices_create(g);
}

/**
 * Frees all memory related to the graph
 */
void graph_destroy(graph *g)
{
	vertex_list_destroy(g->infectious);
	vertex_list_destroy(g->latent);

	int i;
	for(i = 0; i < NETWORK_SIZE; i++)
	{
		vertex_destroy(g->vertices[i]);
		free(g->vertices[i]);
	}
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
		g->vertices[i]->id = i;
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
		iterator = g->vertices[i]->first_neighbor;

		while(iterator != NULL)
		{
			// Generate a random number between 0 and 1
			// if value is less than p, then rewire the edge
			random = (double)rand() / RAND_MAX;

			if(random < p)
			{
				// Keep generating random vertex indices until we find one that we aren't connected to
				do
				{
					// Generate a random vertex index
					random_vertex = ((double)rand() / (double)RAND_MAX) * NETWORK_SIZE;
				} while(random_vertex == i && vertex_edge_exists(g->vertices[i], g->vertices[random_vertex]) == FALSE);

				iterator->vertex = g->vertices[random_vertex];

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

		/*
		if(g->infectious_tail == NULL)
		{
			g->infectious = g->latent;
		}
		else {
			vertex_list_combine(g->infectious_tail, g->latent);
		}

		g->infectious_tail = latent_iterator;
		*/

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

/**
 * Writes out the graph in the pajek file format to the supplied file
 */
void graph_make_pajek(graph *g, char *file)
{
	int i;
	FILE *fp;
	char *color;
	vertex_list_node *iterator;
	
	fp= fopen(file, "w");

	// Print out list of vertices in graph
	fprintf(fp, "*Vertices %i\n", NETWORK_SIZE);

	for(i = 0; i < NETWORK_SIZE; i++)
	{
		switch(g->vertices[i]->state)
		{
			case SUSCEPTIBLE:
				color = "white";
				break;
			case LATENT:
				color = "gray";
				break;
			case INFECTIOUS:
				color = "red";
				break;
			case RECOVERED:
				color = "green";
				break;
		}

		fprintf(fp, "%i \"%i\" ic %s\n", i + 1, i + 1, color);
	}

	fprintf(fp, "*Edges\n");

	for(i = 0; i < NETWORK_SIZE; i++)
	{
		iterator = g->vertices[i]->first_neighbor;

		while(iterator != NULL)
		{
			fprintf(fp, "%i %i\n", i + 1, iterator->vertex->id + 1);
			iterator = iterator->next;
		}
	}

	fclose(fp);
}

/**
 * Opens a CSV statistics file for writing
 * Writes header to file
 */
FILE *graph_open_stats(char *name)
{
	FILE *fp = fopen(name, "w");

	fprintf(fp, "p, k, repitition, day\n");
	
	return fp;
}

/**
 * Appends graph statistics to a supplied file pointer
 */
void graph_write_stats(graph *g, FILE *fp, double p, int k, int r, int day)
{
	fprintf(fp, "%f,%i,%i,%i\n",
		p,
		k,
		r,
		day
	);
}
