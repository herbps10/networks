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

double T = 0.05;
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
  unsigned int seed = time(NULL);
	srand(seed);

  int ring_size = 250;
  int reps = 100;

  graph *g[reps];
  
  for(int i = 0; i < reps; i++)
  {
    g[i] = graph_create();
    graph_circle_first_n_vertices(g[i], ring_size, 2);
  }

  X = T;

  FILE *file = fopen("caveman-stats.csv", "w");

  int write_buffer[100];

  for(int i = 0; i < 36; i++)
  {
    for(int n = 0; n < reps; n++)
    {
      seed++;
      srand(seed);

      graph_add_one_to_clique(g[n], ring_size, i + 1);

      graph_make_susceptible(g[n]);

      graph_init_infected_among_first_n(g[n], ring_size * (i + 1));

      int day = 0;
      while(day < SIMULATION_LENGTH && graph_has_infectious(g[n]) == true)
      {
        graph_advance(g[n], day);

        day++;
      }

      //printf("%i\n", day);

      write_buffer[n] = day;
    }

    for(int n = 0; n < reps; n++)
    {
      fprintf(file, "%i, %i\n", i, write_buffer[n]);
    }

    printf("%i\n", i);
  }

  fclose(file);

}
