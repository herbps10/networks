#include "graph.h"

graph *graph_create()
{
	graph *g = malloc(sizeof(graph));

	graph_init(g);
	graph_reset(g);

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

	graph_reset(g);
}

void graph_reset(graph *g)
{
	graph_circle(g, 2);
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

				vertex_delete_edge(g, g->vertices[i], iterator->vertex);
				vertex_add_edge(g, g->vertices[i], g->vertices[random_vertex]);
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
