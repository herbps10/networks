#ifndef VERTEX_POOL_HEADER
#define VERTEX_POOL_HEADER

struct vertex_pool
{
	vertex_stack *stack;
};

typedef struct vertex_pool vertex_pool;

vertex_node_pool *vertex_pool_create();
void vertex_pool_free(vertex_pool *, vertex *);
vertex *vertex_pool_get(vertex_pool *);

#endif
