#include "node_pool.h"

vertex_node_pool *vertex_node_pool_create()
{
	vertex_node_pool *pool = malloc(sizeof(struct vertex_node_pool_struct));

	pool->stack = vertex_stack_create();

	return pool;
}

void vertex_node_pool_free(vertex_node_pool *pool, struct vertex_node_struct *node)
{
	vertex_stack_push(pool->stack, node);
}

vertex_node *vertex_node_pool_get(vertex_node_pool *pool)
{
	if(vertex_stack_empty(pool->stack))
	{
		vertex_node *node = vertex_node_create();

		return node;
	}
	else
	{
		return vertex_stack_pop(pool->stack);
	}
}
