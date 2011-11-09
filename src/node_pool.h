#ifndef VERTEX_NODE_POOL_HEADER
#define VERTEX_NODE_POOL_HEADER

struct vertex_node_pool_struct
{
	struct vertex_stack_struct *stack;
};

typedef struct vertex_node_pool_struct vertex_node_pool;

struct vertex_node_pool_struct *vertex_node_pool_create();
void vertex_node_pool_free(struct vertex_node_pool_struct *, vertex_node *);
struct vertex_node_struct *vertex_node_pool_get(struct vertex_node_pool_struct *);

#endif
