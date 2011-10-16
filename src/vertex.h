#ifndef VERTEX_HEADER
#define VERTEX_HEADER

#include "graph.h"

enum state_enum {SUSCEPTIBLE, LATENT, INFECTIOUS, RECOVERED};
typedef enum state_enum state;

struct vertex_struct
{
	int id;
	int day;
	enum state_enum state;

	struct vertex_stack_struct *neighbors;
};

typedef struct vertex_struct vertex;

vertex *vertex_create();
void vertex_init();

void vertex_add_edge(struct graph_struct *, vertex *, vertex *);
void vertex_delete_edge(struct graph_struct *, vertex *, vertex *);
_Bool vertex_edge_exists(vertex *, vertex *);
void vertex_add_adjacency(struct graph_struct *, vertex *, vertex *);

#endif
