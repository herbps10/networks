#ifndef GRAPH_HEADER_GUARD
#define GRAPH_HEADER_GUARD

#include "vertex.h"
#include "vertex_list.h"

/**
 * Graph Struct
 *
 */
typedef struct graph graph;
struct graph
{
	vertex_list_node *infectious;
	vertex_list_node *latent;

	vertex *vertices[NETWORK_SIZE];
};

/**
 * Graph Functions
 */

// Creation/Destruction
graph graph_create();
void graph_init(graph *);
void graph_destroy(graph *);

// Convenience Functions
void graph_inspect(graph *g);

// Graph structure
void graph_vertices_create(graph *);
void graph_ring_connect(graph *, int);
void graph_rewire(graph *, double);


// Epidemic
void graph_init_infected(graph *);
int graph_advance(graph *, int);
_Bool graph_has_infectious(graph *);

#endif
