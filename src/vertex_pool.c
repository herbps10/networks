vertex_pool *vertex_pool_create()
{
	vertex_pool *pool = malloc(sizeof(vertex_pool));

	pool->stack = vertex_stack_create();

	return pool;
}

void vertex_pool_free(vertex_pool *pool, vertex *node)
{
	node->backtrace = 0;
	vertex_stack_push(pool->stack, node);
}

vertex *vertex_pool_get(vertex_pool *pool)
{
	if(vertex_stack_empty(pool->stack))
	{
		return vertex_create();
	}
	else
	{
		return vertex_stack_pop(pool->stack);
	}
}
