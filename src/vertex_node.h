#ifndef VERTEX_NODE_HEADER
#define VERTEX_NODE_HEADER

struct vertex_node_struct {
	struct vertex_struct *vertex;
	struct vertex_node_struct *next;
	struct vertex_node_struct *prev;
};

typedef struct vertex_node_struct vertex_node;

// Creation

vertex_node *vertex_node_create();
void vertex_node_init(vertex_node *);

#endif
