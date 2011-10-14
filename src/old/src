#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NETWORK_SIZE 10
#define INITIAL_INFECTIOUS 1

enum state_enum {SUSCEPTIBLE, LATENT, INFECTIOUS, RECOVERED};
typedef enum state_enum state;

struct graph_struct
{
	_Bool *adjacency_matrix;
	state *states;
	short *counter;
};
typedef struct graph_struct graph;

void graph_init(graph *g)
{
	int i, j;

	g->adjacency_matrix = malloc(NETWORK_SIZE * NETWORK_SIZE * sizeof(_Bool));
	g->states = malloc(NETWORK_SIZE * sizeof(state));
	g->counter = malloc(NETWORK_SIZE * sizeof(short));

	for(i = 0; i < NETWORK_SIZE; i++)
	{
		for(j = 0; j < NETWORK_SIZE; j++)
		{
			g->adjacency_matrix[(i * NETWORK_SIZE) + j] = 0;
		}

		g->states[i] = SUSCEPTIBLE;
		g->counter[i] = 0;
	}
}

void graph_init_ring(graph *g, int K)
{
	int row, k, prev, next;
	for(row = 0; row < NETWORK_SIZE; row++)
	{
		for(k = 1; k <= K; k++)
		{
			prev = row - k;
			next = row + k;

			if(prev < 0) prev = NETWORK_SIZE - K;
			if(next >= NETWORK_SIZE) next = K - 1;

			g->adjacency_matrix[(row * NETWORK_SIZE) + prev] = 1;
			g->adjacency_matrix[(row * NETWORK_SIZE) + next] = 1;
		}
	}
}

void graph_edge_add(graph *g, int v1, int v2)
{
	g->adjacency_matrix[(v1 * NETWORK_SIZE) + v2] = 1;
	g->adjacency_matrix[(v2 * NETWORK_SIZE) + v1] = 1;
}

void graph_edge_delete(graph *g, int v1, int v2)
{
	g->adjacency_matrix[(v1 * NETWORK_SIZE) + v2] = 0;
	g->adjacency_matrix[(v2 * NETWORK_SIZE) + v1] = 0;
}

_Bool graph_edge_exists(graph *g, int v1, int v2)
{
	return (g->adjacency_matrix[(v1 * NETWORK_SIZE) + v2] == 1 && g->adjacency_matrix[(v2 * NETWORK_SIZE) + v1]);
}

/**
 * Where p is the rewiring probability
 */
void graph_rewire(graph *g, double p)
{
	int i, j, rewire_vertex;
	double random;

	for(i = 0; i < NETWORK_SIZE; i++)
	{
		for(j = 0; j < NETWORK_SIZE; j++)
		{
			if(g->adjacency_matrix[(i * NETWORK_SIZE) + j] == 1)
			{
				random = (double)rand() / (double)RAND_MAX;

				if(random < p)
				{
					do
					{
						rewire_vertex = ((double)rand() / (double)RAND_MAX) * NETWORK_SIZE;
					} while(graph_edge_exists(g, i, rewire_vertex) == 1 && i != rewire_vertex);

					graph_edge_delete(g, i, j);
					graph_edge_add(g, i, rewire_vertex);
				}
			}
		}
	}
}

void graph_print_adjacency(graph *g)
{
	int i, j;

	printf("  ");
	for(i = 0; i < NETWORK_SIZE; i++)
	{
		printf("%i ", i);
	}
	printf("\n");

	for(i = 0; i < NETWORK_SIZE; i++)
	{
		printf("%i ", i);

		for(j = 0; j < NETWORK_SIZE; j++)
		{
			printf("%i ", g->adjacency_matrix[(i * NETWORK_SIZE) + j]);
		}

		printf("\n");
	}
}

void graph_init_infectious(graph *g)
{
	int num_infectious = 0, random_vertex = 0;

	while(num_infectious < INITIAL_INFECTIOUS)
	{
		random_vertex = rand() % NETWORK_SIZE;
		
		if(g->states[random_vertex] == SUSCEPTIBLE)
		{
			g->states[random_vertex] = INFECTIOUS;
			num_infectious++;
		}
	}
}

void graph_copy(graph *g, graph *n)
{
	int i, j;
	for(i = 0; i < NETWORK_SIZE; i++)
	{
		for(j = 0; j < NETWORK_SIZE; j++)
		{
			n->adjacency_matrix[(i * NETWORK_SIZE) + j] = g->adjacency_matrix[(i * NETWORK_SIZE) + j];
		}
	}
}

void graph_spread_disease(graph *g, graph *n)
{
	int i, j;

	graph_copy(g, n);

	for(i = 0; i < NETWORK_SIZE; i++)
	{
		if(g->states[i] == SUSCEPTIBLE)
		{
			
		}
		else if(g->states[i] == LATENT)
		{
			
		}
		else if(g->states[i] == INFECTIOUS)
		{
			for(j = 0; j < NETWORK_SIZE; j++)
			{
				if(graph_edge_exists(i, j, g))
				{
					
				}
			}
		}
		else if(g->states[i] == RECOVERED)
		{
			
		}

		g->counter[i]++;
	}
}

void graph_destroy(graph *g)
{
	free(g->adjacency_matrix);
	free(g->states);
	free(g->counter);
}

int main()
{
	unsigned int iseed = (unsigned int)time(NULL);
	srand(iseed);

	graph g1;
	graph_init(&g1);

	graph_init_infectious(&g1);
	graph_init_ring(&g1, 1);
	graph_rewire(&g1, 0.05);

	//graph_print_adjacency(&g1);

	graph_destroy(&g1);
	return 0;
}


