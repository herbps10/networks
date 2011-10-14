#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0

#define NETWORK_SIZE 10000
#define SIMULATION_LENGTH 1865
#define INITIAL_INFECTED 5 
#define DAYS_INFECTIOUS 3
#define DAYS_RECOVERED 56
#define R0 2.0

#define NUM_THREADS 4

enum state { SUSCEPTIBLE, LATENT, INFECTIOUS, RECOVERED };
typedef enum state state;

#include "vertex.h"
#include "vertex_list.h"
#include "graph.h"
#include "simulation.h"

#include "vertex_list.c"
#include "graph.c"
#include "vertex.c"
#include "simulation.c"

int main()
{	
	srand(time(NULL));

	vertex v1 = vertex_create();
	vertex v2 = vertex_create();

	vertex_list_node *head1 = malloc(sizeof(vertex_list_node));

	head1->vertex = &v1;
	head1->prev = NULL;
	head1->next = NULL;

	printf("Vertex List 1\n");
	head1 = vertex_list_append(head1, &v2);

	vertex_list_inspect(head1);



	vertex v3 = vertex_create();
	vertex v4 = vertex_create();

	vertex_list_node *head2 = malloc(sizeof(vertex_list_node));
	head2->vertex = &v3;

	head2 = vertex_list_append(head2, &v3);

	printf("Vertex List 2\n");
	vertex_list_inspect(head2);

	printf("Combined\n");
	vertex_list_node *merged1 = vertex_list_merge(head2, head1);
	vertex_list_inspect(merged1);

	/*
	printf("Combined right null\n");
	vertex_list_node *merged2 = vertex_list_merge(head1, NULL);
	vertex_list_inspect(merged2);

	printf("Combined left null\n");
	vertex_list_node *merged3 = vertex_list_merge(NULL, head2);
	vertex_list_inspect(merged3);
	*/

	return 0;
}
