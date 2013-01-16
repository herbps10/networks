#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NETWORK_SIZE 10000
#define INITIAL_INFECTED 5

#define DAYS_INFECTIOUS 3
#define DAYS_LATENT 1
#define DAYS_RECOVERED 60

#define REPITITIONS 10

#define R0 3

#define RNOT_SCHEME 0

#define SIMULATION_LENGTH 1865

#define WRITE_BUFFER_SIZE 5000

double T = 0.5;
double R_not = 1;
double X;
int k;

// from http://stackoverflow.com/questions/2999075/generate-a-random-number-within-range/2999130#2999130
int rand_lim(int limit) {
/* return a random number between 0 and limit inclusive.
 */

    int divisor = RAND_MAX/(limit+1);
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}

double rand_one()
{
  return (double)rand() / (double)RAND_MAX;
}

#include "vertex_node.c"
#include "stack.c"
#include "queue.c"
#include "node_pool.c"
#include "vertex.c"
#include "graph.c"

int main(int argc, char *argv[])
{
	srand(time(NULL));

	graph *g = graph_create();

  X = T;

  FILE *file = fopen("preferential-stats.csv", "w");

  // The first edge is always the same (vertex 0 to vertex 1).
  // We add this one first manually to kick off the process.
  vertex_add_edge(g, g->vertices[0], g->vertices[1]);

  int edges = 2;

  // Now we loop through from 2 to the total number of nodes.
  // At each iteration, i is the index of the vertex that we are to add next,
  // and 0-(i-1) are the nodes that are already connected.
  for(int i = 2; i < NETWORK_SIZE * 4; i++)
  {
    if(i >= NETWORK_SIZE)
    {
      double random_number = ((double)rand() / (double)RAND_MAX) * (edges - g->vertices[i % NETWORK_SIZE]->degree);

      int node2;
      for(int n = 0; n < NETWORK_SIZE; n++)
      {
        if(n == (i % NETWORK_SIZE))
        {
          continue;
        }

        if(random_number < g->vertices[n]->degree)
        {
          node2 = n;
          break;
        }

        random_number -= g->vertices[n]->degree;
      }

      vertex_add_edge(g, g->vertices[i % NETWORK_SIZE], g->vertices[node2]);
    }
    else
    {
      // Add a preferential edge
      double random_number = ((double)rand() / (double)RAND_MAX) * edges;

      int node2;

      for(int n = 0; n < i; n++) {
        if(random_number < g->vertices[n]->degree) {
          node2 = n;
          break;
        }

        random_number -= g->vertices[n]->degree;
      }

      vertex_add_edge(g, g->vertices[i], g->vertices[node2]);
    }

    for(int j = 0; j < REPITITIONS; j++)
    {
      graph_make_susceptible(g);

      graph_init_infected(g);

      int day = 0;
      while(day < SIMULATION_LENGTH && graph_has_infectious(g) == true)
      {
        graph_advance(g, day);

        day++;
      }

      fprintf(file, "%i, %i\n", i, day);
    }

    printf("%i\n", i);

    edges += 2;
  }

  //graph_write_pajek(g, "preferential-attachment.pajek");

  fclose(file);
}
