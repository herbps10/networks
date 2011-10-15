vertex_node_pool *vertex_node_pool_create()
{
	vertex_node_pool *pool = malloc(sizeof(vertex_node_pool));

	pool->stack = vertex_stack_create();

	return pool;
}

void vertex_node_pool_free(vertex_node_pool *pool, vertex_node *node)
{
	vertex_stack_push(pool->stack, node);
}

vertex_node *vertex_node_pool_get(vertex_node_pool *pool)
{
	if(vertex_stack_empty(pool->stack))
	{
		return vertex_node_create();
	}
	else
	{
		return vertex_stack_pop(pool->stack);
	}
}
