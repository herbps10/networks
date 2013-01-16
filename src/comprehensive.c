#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define INITIAL_INFECTED 5

#define RNOT_SCHEME 0

#define SIMULATION_LENGTH 1865

#define NETWORK_SIZE 1000

double T = 0.05;
double X;
int k;
int DAYS_INFECTIOUS;
int DAYS_RECOVERED;

#include "random.c"
#include "vertex_node.c"
#include "stack.c"
#include "queue.c"
#include "node_pool.c"
#include "vertex.c"
#include "graph.c"

int days_of_infected(graph *g)
{
  int day = 0;
  while(day < SIMULATION_LENGTH && graph_has_infectious(g) == true)
  {
    graph_advance(g, day);
    day++;
  }

  return day;
}

int main(int argc, char *argv[])
{
  // Set the random seed
  unsigned int seed = time(NULL);
  srand(seed);
  
  graph *test_graph = graph_create();
  int days;
  
  FILE *log = fopen("comprehensive-experiment-results.csv", "w");
  fprintf(log, "type, days, days_infectious, days_recovered, T, p, k, network_size, seed\n");

  for(DAYS_INFECTIOUS = 1; DAYS_INFECTIOUS <= 2; DAYS_INFECTIOUS++)
  {
    for(DAYS_RECOVERED = 1; DAYS_RECOVERED <= 2; DAYS_RECOVERED++)
    {
      for(T = 0.05; T <= 1.0; T += 0.5)
      {
        X = T;

        // Test the random graph
        graph_reset(test_graph);
        for(int i = 0; i < NETWORK_SIZE; i++)
        {
          graph_add_random_edge(test_graph);
        }
        graph_init_infected(test_graph);

        days = days_of_infected(test_graph);
        fprintf(log, "random, %i, %i, %i, %f, 0, 0, %i, %i\n", days, DAYS_INFECTIOUS, DAYS_RECOVERED, T, NETWORK_SIZE, seed);

        // Test the Barabasi-Alberts Preferential Attachment Graph
        graph_reset(test_graph);
        graph_barabasi(test_graph, 1.0, 1);
        graph_init_infected(test_graph);

        days = days_of_infected(test_graph);
        fprintf(log, "barabasi, %i, %i, %i, %f, 0, 0, %i, %i\n", days, DAYS_INFECTIOUS, DAYS_RECOVERED, T, NETWORK_SIZE, seed);

        // Test the Watts-Strogatz Graph
        for(double p = 0; p <= 1.0; p += 0.5)
        {
          for(int k = 1; k < 2; k++)
          {
            graph_reset(test_graph);
            graph_circle(test_graph, k);
            graph_rewire(test_graph, p);
            graph_init_infected(test_graph);

            days = days_of_infected(test_graph);
            fprintf(log, "watts, %i, %i, %i, %f, %f, %i, %i, %i\n", days, DAYS_INFECTIOUS, DAYS_RECOVERED, T, p, k, NETWORK_SIZE, seed);
          }
        }


        seed += 1;
        srand(seed);

        printf("%i, %i, %f\n", DAYS_INFECTIOUS, DAYS_RECOVERED, T);
      }
    }
  }

  return 0;
}
