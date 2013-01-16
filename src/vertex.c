#include "vertex.h"

vertex *vertex_create()
{
	vertex *v = malloc(sizeof(struct vertex_struct));

	vertex_init(v);

	return v;
}

void vertex_init(vertex *v)
{
	v->id = 0;
	v->state = SUSCEPTIBLE;
	v->day = 0;
	v->degree = 0;
	v->times_sick = 0;

	v->neighbors = vertex_stack_create();
}

void vertex_add_edge(graph *g, vertex *v1, vertex *v2)
{
	v1->degree++;
	v2->degree++;
	vertex_add_adjacency(g, v1, v2);
	vertex_add_adjacency(g, v2, v1);
}

void vertex_add_adjacency(graph *g, vertex *v1, vertex *v2)
{
	vertex_node *link = vertex_node_pool_get(g->pool);
	link->vertex = v2;
	vertex_stack_push(v1->neighbors, link);
}

_Bool vertex_edge_exists(vertex *v1, vertex *v2)
{
	vertex_node *iterator = v1->neighbors->head;
	while(iterator != NULL)
	{
		if(iterator->vertex == v2)
		{
			return true;
		}

		iterator = iterator->next;
	}

	return false;
}

void vertex_delete_edge(struct graph_struct *g, vertex *v1, vertex *v2)
{
	v1->degree--;
	v2->degree--;

	vertex_node *iterator = v1->neighbors->head;

	while(iterator != NULL)
	{
		if(iterator->vertex == v2)
		{
			if(iterator->prev == NULL)
			{
				v1->neighbors->head = iterator->next;
			}
			else 
			{
				iterator->prev->next = iterator->next;
			}

			iterator->vertex = NULL;

			vertex_node_pool_free(g->pool, iterator);
		}

		iterator = iterator->next;
	}
}

void vertex_set_state(vertex *v, state s, int day)
{
	v->state = s;
	v->day = day;
}

void vertex_infect_neighbors(graph *g, vertex *v, int day)
{
	vertex_node *iterator;

	// Sanity check
	// The vertex can only infect others if it is infectious
	// Also if it was just set to the infectious state this turn then it can't infect anyone else
	if(v->state == INFECTIOUS && day - v->day > 0)
	{
		// Go through each neighbor
		iterator = v->neighbors->head;

		while(iterator != NULL)
		{
			// If the vertex is recovered, then there is a chance
			// that it is ready to move back into the susceptible class.
			//
			// Check to see if it should be moved back into the susceptible class.
			if(iterator->vertex->state == RECOVERED)
			{
				if(day - iterator->vertex->day > DAYS_RECOVERED)
				{
					vertex_set_state(iterator->vertex, SUSCEPTIBLE, day);
				}
			}
			
			// If the neighbor is susceptible, try to infect it!
			if(iterator->vertex->state == SUSCEPTIBLE)
			{
				// If the disease was spread to the neighbor, 
				// then add it to the list of newly infected vertices
				vertex_spread_infection(g, v, iterator->vertex, day);
			}
			

			iterator = iterator->next;
		}
	}
}

void vertex_spread_infection(graph *g, vertex *source, vertex *target, int day)
{
	double T, r;
	
	if(RNOT_SCHEME == 0)
	{
		T = X;
	}
	else 
	{
		T = 1.0 - pow(1.0 - X/(double)k, 1.0 / (double)DAYS_INFECTIOUS);
		//T = 0.333 - (1.0/180.0) * (double)k;
		//T = (1.0/180.0) * (double)k;
	}

	// The probability of infecting a neighbor is proportional to
	// how the degree of the vertex. This is so that the number of
	// of secondary infections caused by the vertex will roughly equal
	// R0.
	/*if(RNOT_SCHEME == 1)
	{
		T = 1 - (1 - (((double)X/(double)source->degree) / (double)DAYS_INFECTIOUS));
	}
	*/

	// Generate a random number between 0 and 1
	// If it is smaller than T, then spread the infection
	r = (double)rand() / (double)RAND_MAX;

	// If we infect the neighbor, put them into the
	// latent class
	if(r < T)
	{
		//vertex_set_state(target, LATENT, day);
    vertex_set_state(target, INFECTIOUS, day);

		vertex_node *node = vertex_node_pool_get(g->pool);
		node->vertex = target;

		vertex_queue_enqueue(g->infectious, node);

		node->vertex->times_sick++;

		// Record that we caused an infection
		source->infections_caused++;
	}
}
