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

	v->neighbors = vertex_stack_create();
}

void vertex_add_edge(graph *g, vertex *v1, vertex *v2)
{
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
