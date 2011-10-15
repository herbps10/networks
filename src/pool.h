#ifndef VERTEX_NODE_POOL_HEADER
#define VERTEX_NODE_POOL_HEADER

struct vertex_node_pool
{
	vertex_stack *stack;
};

typedef struct vertex_node_pool vertex_node_pool;

vertex_node_pool *vertex_node_pool_create();
void vertex_node_pool_free(vertex_node_pool *, vertex_node *);
vertex_node *vertex_node_pool_get(vertex_node_pool *);

#endif
