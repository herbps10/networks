#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NETWORK_SIZE 10000
#define INITIAL_INFECTED 5

#define REPITITIONS 10

#define R0 3

#define RNOT_SCHEME 0

#define SIMULATION_LENGTH 1865

#define WRITE_BUFFER_SIZE 5000

double T = 0.05;
double R_not = 1;
double X;
int k;

int DAYS_INFECTIOUS = 3;
int DAYS_LATENT = 1;
int DAYS_RECOVERED = 60;

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

  graph *g = graph_create();
  
  X = T;

  FILE *file = fopen("caveman-stats.csv", "w");

  fprintf(file, "k, p, days_infectious, days_latent, days_recovered, day\n");

  int counter = 0;

  for(int k = 2; k < 20; k++)
  {
    seed++;
    srand(seed);

    for(double p = 0; p < 1; p += 0.01)
    {
      for(int infectious_period = 1; infectious_period < 10; infectious_period++)
      {
        for(int latent_period = 1; latent_period < 10; latent_period++)
        {
          for(int recovered_period = 1; recovered_period < 10; recovered_period++)
          {
            DAYS_INFECTIOUS = infectious_period;
            DAYS_LATENT = latent_period;
            DAYS_RECOVERED = latent_period;


            graph_reset(g);
            graph_circle(g, k);
            graph_rewire(g, p);
            graph_init_infected(g);

            int day = 0;
            while(day < SIMULATION_LENGTH && graph_has_infectious(g) == true)
            {
              graph_advance(g, day);

              day++;
            }

            //printf("%i\n", day);
            fprintf(file, "%i, %f, %i, %i, %i, %i\n", k, p, infectious_period, latent_period, recovered_period, day);

            counter++;
          }
        }
      }
      printf("p: %f\n", p);
    }


    printf("k: %i\n", k);
  }

  fclose(file);

}
