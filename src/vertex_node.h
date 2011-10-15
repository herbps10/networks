#ifndef VERTEX_NODE_HEADER
#define VERTEX_NODE_HEADER

struct vertex_node {
	//struct vertex *vertex;
	int vertex;
	struct vertex_node *next;
	struct vertex_node *prev;
};

typedef struct vertex_node vertex_node;

// Creation

vertex_node *vertex_node_create();
void vertex_node_init(vertex_node *);

#endif
