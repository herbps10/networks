#ifndef VERTEX_NODE_HEADER
#define VERTEX_NODE_HEADER

#include "vertex.h"

typedef struct vertex_node vertex_node;

struct vertex_node {
	vertex *vertex;
	vertex_node *next;
	vertex_node *prev;
};


// Creation

vertex_node *vertex_node_create();
void vertex_node_init(vertex_node *);

#endif
