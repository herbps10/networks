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

void graph_reset(graph *g)
{
	for(int i = 0; i < NETWORK_SIZE; i++)
	{
		g->vertices[i]->state = SUSCEPTIBLE;

		vertex_stack_destroy(g, g->vertices[i]->neighbors);

		g->vertices[i]->degree = 0;
		g->vertices[i]->day = 0;
	}

	vertex_queue_destroy(g, g->infectious);
	vertex_queue_destroy(g, g->latent);
}

void graph_circle(graph *g, int K)
{
	int before, after;
	for(int i = 0; i < NETWORK_SIZE; i++)
	{
		for(int k = 1; k <= K; k++)
		{
			before = i - k; // The vertex to connect to on the left of the current vertex (smaller index)
			after = i + k; // The vertex to connect to on the right of the current vertex (larger index)

			if(before < 0) before += NETWORK_SIZE; // If we're less than 0 wrap around to the other end
			if(after >= NETWORK_SIZE) after -= NETWORK_SIZE; // If we've gone past the end of the array wrap around to the other end

			vertex_add_edge(g, g->vertices[i], g->vertices[before]);
			vertex_add_edge(g, g->vertices[i], g->vertices[after]);
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

				iterator->vertex = g->vertices[random_vertex];

				//vertex_delete_edge(g, g->vertices[i], iterator->vertex);
				//vertex_add_edge(g, g->vertices[i], g->vertices[random_vertex]);
			}

			iterator = iterator->next;
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
	vertex_node *node;

	node = vertex_queue_dequeue(g->latent);
		
	while(node != NULL)
	{
		vertex_set_state(node->vertex, INFECTIOUS, day);
		vertex_queue_enqueue(g->infectious, node);

		node = vertex_queue_dequeue(g->latent);
	}

	/**
	 * Now loop through all the remaining infectious vertices and
	 * have them try to infect their neighbors.
	 */
	
	vertex_node *infectious_iterator = g->infectious->head;
	while(infectious_iterator != NULL)
	{
		vertex_infect_neighbors(g, infectious_iterator->vertex, day);

		infectious_iterator = infectious_iterator->next;
	}
}

/**
 * Writes out the graph in the pajek file format to the supplied file
 */
void graph_write_pajek(graph *g, char *file)
{
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
			fprintf(fp, "%i %i\n", i + 1, iterator->vertex->id + 1);
			iterator = iterator->next;
		}
	}

	fclose(fp);
}

_Bool graph_has_infectious(graph *g)
{
	return !vertex_queue_empty(g->infectious);
}
