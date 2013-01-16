#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NETWORK_SIZE 5000
#define INITIAL_INFECTED 5

#define DAYS_INFECTIOUS 5 //3
#define DAYS_LATENT 2 //1
#define DAYS_RECOVERED 20 //60

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
  unsigned int seed = time(NULL);
	srand(seed);

  graph *g[100];

  for(int i = 0; i < 100; i++)
  {
    g[i] = graph_create();
  }

  X = T;

  FILE *file = fopen("random-stats-100-graphs-5000-n-other-params.csv", "w");

  for(int i = 2000; i < 8000; i++)
  {
    for(int n = 0; n < 100; n++)
    {
      seed++;
      srand(seed);

      graph_add_random_edge(g[n]);

      graph_make_susceptible(g[n]);

      graph_init_infected(g[n]);

      int day = 0;
      while(day < SIMULATION_LENGTH && graph_has_infectious(g[n]) == true)
      {
        graph_advance(g[n], day);

        day++;
      }

      //printf("%i\n", day);

      fprintf(file, "%i, %i\n", i, day);
    }

    printf("%i\n", i);
  }

  fclose(file);

}
