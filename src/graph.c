#include "graph.h"

graph *graph_create()
{
	graph *g = malloc(sizeof(graph));

	graph_init(g);

	return g;
}

void graph_init(graph *g)
{
	for(int i = 0; i < NETWORK_SIZE; i++)
	{
		g->vertices[i] = vertex_create();
		g->vertices[i]->id = i;
	}

	g->infectious = vertex_queue_create();
	g->latent = vertex_queue_create();

	g->pool = vertex_node_pool_create();
}

void graph_make_susceptible(graph *g)
{
    for(int i = 0; i < NETWORK_SIZE; i++)
    {
      g->vertices[i]->state = SUSCEPTIBLE;
      g->vertices[i]->times_sick = 0;
      g->vertices[i]->infections_caused = 0;
    }
}

void graph_reset(graph *g)
{
	for(int i = 0; i < NETWORK_SIZE; i++)
	{
		g->vertices[i]->state = SUSCEPTIBLE;

		vertex_stack_destroy(g, g->vertices[i]->neighbors);

		g->vertices[i]->degree = 0;
		g->vertices[i]->day = 0;
		g->vertices[i]->times_sick = 0;
		g->vertices[i]->infections_caused = 0;
	}

	vertex_queue_destroy(g, g->infectious);
	vertex_queue_destroy(g, g->latent);
}

void graph_add_random_edge(graph *g)
{
  int node1, node2;
  node1 = rand_lim(NETWORK_SIZE - 1);

  do
  {
    node2 = rand_lim(NETWORK_SIZE - 1);
  }
  while(node2 == node1);

  vertex_add_edge(g, g->vertices[node1], g->vertices[node2]);
}

void graph_circle(graph *g, int K)
{
	int after;

	for(int k = 1; k <= ceil((K+1)/2); k++)
	{
		for(int i = 0; i < NETWORK_SIZE; i++)
		{
			after = i + k;
			if(after >= NETWORK_SIZE) after %= NETWORK_SIZE;
			
			if(g->vertices[after]->degree < K && g->vertices[i]->degree < K)
			{
				vertex_add_edge(g, g->vertices[i], g->vertices[after]);
			}
		}
	}
}

void graph_circle_first_n_vertices(graph *g, int N, int K)
{
	int after;

	for(int k = 1; k <= ceil((K+1)/2); k++)
	{
		for(int i = 0; i < N; i++)
		{
			after = i + k;
			if(after >= N) after %= N;
			
			if(g->vertices[after]->degree < K && g->vertices[i]->degree < K)
			{
				vertex_add_edge(g, g->vertices[i], g->vertices[after]);
			}
		}
	}
}

void graph_add_one_to_clique(graph *g, int ring_size, int time) {
  for(int i = 0; i < ring_size; i++)
  {
    for(int n = time - 1; n >= 0; n--)
    {
      vertex_add_edge(g, g->vertices[n * ring_size + i], g->vertices[time * ring_size + i]);
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
	vertex_node *iterator;
	
	for(i = 0; i < NETWORK_SIZE; i++)
	{
		iterator = g->vertices[i]->neighbors->head;

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
				} while(random_vertex == i && vertex_edge_exists(g->vertices[i], g->vertices[random_vertex]) == false);

        // This is the part that does the actual rewiring
				iterator->vertex = g->vertices[random_vertex];

				//vertex_delete_edge(g, g->vertices[i], iterator->vertex);
				//vertex_add_edge(g, g->vertices[i], g->vertices[random_vertex]);
			}

			iterator = iterator->next;
		}
	}
}

void graph_barabasi(graph *g, float power, float m)
{
  // The first edge is always the same (vertex 0 to vertex 1).
  // We add this one first manually to kick off the process.
  vertex_add_edge(g, g->vertices[0], g->vertices[1]);

  float degree_sum = 2 * pow(1, power); // Sum of degree of each vertex in graph

  for(int i = 2; i < NETWORK_SIZE; i++)
  {
    // Add m edges for each new vertex we bring into the graph
    for(int r = 0; r < m; r++)
    {
      int node2; // The node to attach to

      // Generate a random number from 0 to degree_sum
      double random_number = ((double)rand() / (double)RAND_MAX) * degree_sum;

      // Choose the node to attach to preferentially based on each vertex's degree
      for(int n = 0; n < i; n++)
      {
        if(random_number < pow(g->vertices[n]->degree, power))
        {
          node2 = n;
          break;
        }

        random_number -= pow(g->vertices[n]->degree, power);
      }

      float prev_degree_1 = pow(g->vertices[i]->degree, power);
      float prev_degree_2 = pow(g->vertices[node2]->degree, power);

      vertex_add_edge(g, g->vertices[i], g->vertices[node2]);

      float after_degree_1 = pow(g->vertices[i]->degree, power);
      float after_degree_2 = pow(g->vertices[node2]->degree, power);

      if(power > 0)
      {
        degree_sum += (after_degree_1 - prev_degree_1) + (after_degree_2 - prev_degree_2);
      }
      else
      {
        // If power = 0, then the above formula won't add anything as the difference will be zero. (the degree before to the zero is one, and the degree after to the zero is also one, so the difference is zero). So we have to handle
        // this delicately -- basically if we just added a new vertex, then we should add one to the degree sum.
        // Otherwise, it stays the same.
        if(r == 0)
        {
          degree_sum += 1;
        }
      }
    }
  }
}

void graph_inspect(graph *g)
{
	for(int i = 0; i < NETWORK_SIZE; i++)
	{
		printf("Vertex: %i\n", g->vertices[i]->id);
	}
}

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

			vertex_node *node = vertex_node_pool_get(g->pool);
			node->vertex = g->vertices[r];

			// Add this individual to the graph's list of infected individuals
			vertex_queue_enqueue(g->infectious, node);

			infected++;
		}
	}
	while(infected < INITIAL_INFECTED);
}


void graph_init_infected_among_first_n(graph *g, int N)
{
	int infected = 0, r;
	do
	{
		r = ((double)rand() / (double)RAND_MAX) * N;
		
		// Check to make sure the vertex isn't already infectious
		if(g->vertices[r]->state != INFECTIOUS)
		{
			// This is always going to be used at the start of a new simulation,
			// so days will be 0
			vertex_set_state(g->vertices[r], INFECTIOUS, 0);

			vertex_node *node = vertex_node_pool_get(g->pool);
			node->vertex = g->vertices[r];

			// Add this individual to the graph's list of infected individuals
			vertex_queue_enqueue(g->infectious, node);

			infected++;
		}
	}
	while(infected < INITIAL_INFECTED);
}

void graph_advance(graph *g, int day)
{
	vertex_node *infected_node;

	/**
	 * Loop through the end of the infected queue
	 * and look for ones that shouldn't be infectious any longer.
	 * remove them. When it hits one that should remain infectious,
	 * it stops.
	 */
	while(vertex_queue_top(g->infectious) != NULL)
	{
		if(vertex_queue_top(g->infectious)->vertex == NULL)
		{
			printf("queue top = null\n");
			continue;
		}

		if(vertex_queue_top(g->infectious)->vertex->state == LATENT)
		{
			vertex_set_state(vertex_queue_top(g->infectious)->vertex, INFECTIOUS, day);
			vertex_queue_top(g->infectious)->vertex->times_sick++;
		}

		if(day - vertex_queue_top(g->infectious)->vertex->day > DAYS_INFECTIOUS)
		{
			infected_node = vertex_queue_dequeue(g->infectious);
			vertex_set_state(infected_node->vertex, RECOVERED, day);
			vertex_node_pool_free(g->pool, infected_node);
		}
		else
		{
			break;
		}
	}

	/**
	 * Change over all the latent to infectious
	 */
	vertex_queue_prepend(g->infectious, g->latent);
	g->latent->head = NULL;
	g->latent->tail = NULL;
	g->latent->length = 0;

	/**
	 * Now loop through all the remaining infectious vertices and
	 * have them try to infect their neighbors.
	 */
	
	vertex_node *infectious_iterator = g->infectious->head;
	while(infectious_iterator != NULL)
	{
		if(infectious_iterator->vertex->state == LATENT)
		{
			vertex_set_state(infectious_iterator->vertex, INFECTIOUS, day);
			infectious_iterator->vertex->times_sick++;
		}

		vertex_infect_neighbors(g, infectious_iterator->vertex, day);

		infectious_iterator = infectious_iterator->next;
	}
}

/**
 * Writes out the graph in the pajek file format to the supplied file
 */
void graph_write_pajek(graph *g, char *file)
{
  int printed[NETWORK_SIZE][NETWORK_SIZE];

  for(int i = 0; i < NETWORK_SIZE; i++)
  {
    for(int j = 0; j < NETWORK_SIZE; j++)
    {
      printed[i][j] = 0;
    }
  }

	int i;
	FILE *fp;
	char *color;
	vertex_node *iterator;
	
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
		iterator = g->vertices[i]->neighbors->head;

		while(iterator != NULL)
		{
      if(printed[i][iterator->vertex->id] == 0 && printed[iterator->vertex->id][i] == 0)
      {
			  fprintf(fp, "%i %i\n", i + 1, iterator->vertex->id + 1);

        printed[i][iterator->vertex->id] = 1;
        printed[iterator->vertex->id][i] = 1;
      }

			iterator = iterator->next;
		}
	}

	fclose(fp);
}

_Bool graph_has_infectious(graph *g)
{
	return !vertex_queue_empty(g->infectious);
}

/**
 * Opens a CSV statistics file for writing
 * Writes header to file
 */
FILE *graph_open_stats(char *name)
{
	FILE *fp = fopen(name, "w");

	fprintf(fp, "T, p, k, repitition, day, times_sick, max_infected, peak\n");
	
	return fp;
}

/**
 * Appends graph statistics to a supplied file pointer
 */
void graph_write_stats(graph *g, FILE *fp, double T, double p, int k, int r, int day, double average_times_sick, double max_infected, int peak)
{
	fprintf(fp, "%f,%f,%i,%i,%i,%f,%f,%i\n",
		T,
		p,
		k,
		r,
		day,
		average_times_sick,
		max_infected,
		peak
	);
}

/**
 * Averages the number of times each vertex gets sick
 */
double graph_average_times_sick(graph *g)
{
	int sum = 0;
	for(int i = 0; i < NETWORK_SIZE; i++)
	{
		sum += g->vertices[i]->times_sick;
	}

	return (double)sum/(double)NETWORK_SIZE;
}

/**
 * Finds the maximum number of people infected at one time
 */
double graph_max_infected(graph *g)
{
	int sum = 0;

	for(int i = 0; i < NETWORK_SIZE; i++)
	{
		if(g->vertices[i]->times_sick > 0) sum += 1;
	}

	return sum;
}

/**
 * Counts the number of individuals in each class
 */
int *graph_get_counts(graph *g)
{
	int *counts = malloc(sizeof(int) * 4);
	
	for(int i = 0; i < NETWORK_SIZE; i++)
	{
		counts[g->vertices[i]->state]++;
	}

	return counts;
}

/**
 * Counts number of infected
 */
int graph_get_infected(graph *g)
{
	int count = 0;

	vertex_node *iterator = g->infectious->head;
	while(iterator != NULL)
	{
		count++;
	}

	return count;
}
