#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NETWORK_SIZE 10000
#define INITIAL_INFECTED 5

#define DAYS_INFECTIOUS 3
#define DAYS_LATENT 1
#define DAYS_RECOVERED 60

#define REPITITIONS 30

#define R0 2

#define RNOT_SCHEME 0

#define SIMULATION_LENGTH 10000

double T = 0.025;
double R_not = 1;
double X;
int k;

#include "vertex_node.c"
#include "stack.c"
#include "queue.c"
#include "node_pool.c"
#include "vertex.c"
#include "graph.c"

int main(int argc, char *argv[])
{
	srand(time(NULL));

  /*
	if(argc != 3)
	{
		printf("Usage: one p K\n");
		return 0;
	}

	float p;

	sscanf(argv[1], "%f", &p);
	sscanf(argv[2], "%i", &k);
  */

	graph *g = graph_create();


  FILE *file;

  
  for(k = 38; k < 40; k++)
  {
    for(double p = 0.0; p <= 1; p += 0.01)
    {
      printf("%i, %f\n", k, p);
      for(int repitition = 0; repitition < REPITITIONS; repitition++)
      {
        char file_str[300];

        graph_reset(g);

        graph_circle(g, k);
        graph_rewire(g, p);
        graph_init_infected(g);


        X = 0.025;

        int day = 0;
        //char file[100];
        int *counts;

        snprintf(file_str, 300, "data/many/stats-%i-%4.2f-%i.csv", k, p, repitition);

        /*printf("%s\n", file_str);*/


        FILE *file = fopen(file_str, "w");

        fprintf(file, "susceptible, latent, infectious, recovered\n");

        /*
        FILE *file = fopen("data/infectious_counts.csv", "w");
        fprintf(file, "vertex, count\n");
        */

        //FILE *infection_file = fopen("data/infection_events.csv", "w");
        //fprintf(infection_file, "time, vertex\n");

        int infectious_counts[NETWORK_SIZE];
        int infectious_tracker[NETWORK_SIZE];
        
        for(int i = 0; i < NETWORK_SIZE; i++)
        {
          infectious_counts[i] = 0;
          infectious_tracker[i] = 0;
        }

        vertex_node *iterator;

        while(day < SIMULATION_LENGTH && graph_has_infectious(g) == true)
        {
          graph_advance(g, day);

          //snprintf(file, 100, "data/one/%i.pajek", day);
          //graph_write_pajek(g, file);

          counts = graph_get_counts(g);

          fprintf(file, "%i, %i, %i, %i\n", counts[SUSCEPTIBLE], counts[LATENT], counts[INFECTIOUS], counts[RECOVERED]);

          for(int i = 0; i < NETWORK_SIZE; i++)
          {
            if(g->vertices[i]->state == INFECTIOUS && infectious_tracker[i] == 1)
            {
              continue;
            }

            if(g->vertices[i]->state == INFECTIOUS)
            {
              infectious_counts[i]++;
              infectious_tracker[i] = 1;
            }
            
            if(g->vertices[i]->state != INFECTIOUS)
            {
              infectious_tracker[i] = 0;
            }
          }

          
          /*
          for(int i = 0; i < NETWORK_SIZE; i++)
          {
            if(g->vertices[i]->day == day && g->vertices[i]->state == INFECTIOUS)
            {
              fprintf(infection_file, "%i, %i\n", day, i);
            }
          }
          */

          day++;
        }
        
        fclose(file);
      }
    }
  }

	/*
	for(int i = 0; i < NETWORK_SIZE; i++)
	{
		fprintf(file, "%i, %i\n", i, infectious_counts[i]);
	}
	*/
	
	//fclose(infection_file);
}
