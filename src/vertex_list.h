#ifndef VERTEX_LIST_HEADER_GUARD
#define VERTEX_LIST_HEADER_GUARD

#include "vertex.h"

/**
 * Vertex List Node Struct
 *
 * Linked list node structure for use in an individual's
 * adjacency list. Also used to keep track of all the
 * infected individuals.
 */
typedef struct vertex_list_node vertex_list_node;
struct vertex_list_node
{
	struct vertex *vertex;
	struct vertex_list_node *prev;
	struct vertex_list_node *next;
};

/**
 * Vertex List Functions
 */
void vertex_list_destroy(struct vertex_list_node *);
void vertex_list_destroy_vertices(struct vertex_list_node *);
int vertex_list_contains(struct vertex_list_node *, struct vertex *);
struct vertex_list_node *vertex_list_append(struct vertex_list_node *, struct vertex *);
struct vertex_list_node *vertex_list_merge(struct vertex_list_node *, struct vertex_list_node *);
void vertex_list_combine(vertex_list_node *, vertex_list_node *);
void vertex_list_inspect(vertex_list_node *);

#endif
