#ifndef VERTEX_HEADER_GUARD
#define VERTEX_HEADER_GUARD

#include "vertex_list.h"

/**
 * Vertex Struct
 *
 * Holds all the data for an individual
 */
typedef struct vertex vertex;
struct vertex
{
	int id; // The vertex id, assigned at the initialization of the graph
	int state_day; // The day in which the current vertex state was set
	enum state state; // The current state of the individual
	
	struct linked_array neighbors; // A linked array of neighbors

	int degree; // The number of neighbors the vertex has
};



/**
 * Vertex Functions
 */

// Creation/Destruction
vertex vertex_create();
void vertex_init(vertex *);
void vertex_destroy(vertex *);

// Edges/Adjacency
void vertex_add_edge(vertex *, vertex *);
void vertex_delete_edge(vertex *, vertex *);

void vertex_add_adjacency(vertex *, vertex *);
void vertex_delete_adjacency(vertex *, vertex *);

short vertex_edge_exists(vertex *, vertex *);

// State
void vertex_set_state(vertex *, state, int);

// Epidemic
vertex_list_node *vertex_infect_neighbors(vertex *, int);
_Bool vertex_spread_infection(vertex *, vertex *, int);

#endif
