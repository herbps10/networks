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

  FILE *file = fopen("barabasi-stats", "w");

  fprintf(file, "T, power, k, day\n");

  for(T = 0.01; T < 1; T += 0.05)
  {
    printf("T: %f\n", T);

    X = T;
    for(float power = 0; power < 1; power += 0.1)
    {
      printf("Power: %f\n", power);

      for(int k = 1; k < 4; k++)
      {
        printf("k: %i\n", k);
        graph_reset(g);

        graph_barabasi(g, power, k);

        graph_init_infected(g);

        int day = 0;

        while(day < SIMULATION_LENGTH && graph_has_infectious(g) == true)
        {
          graph_advance(g, day);

          day++;
        }

        fprintf(file, "%f, %f, %i, %i\n", T, power, k, day);
      }

    }
  }

  return 0;
}
