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
typedef struct vertex_list vertex_list;
struct vertex_list
{
	struct vertex *list[NETWORK_SIZE];
	int start;
	int end;
};

/**
 * Vertex List Functions
 */
int vertex_list_contains(struct vertex_list *, struct vertex *);
void vertex_list_add(struct vertex_list *, struct vertex *);
void vertex_list_append(vertex_list *, vertex_list *);
void vertex_list_inspect(vertex_list *);

#endif
