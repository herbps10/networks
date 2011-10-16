#include "vertex.h"

vertex *vertex_create()
{
	vertex *v = malloc(sizeof(vertex));

	vertex_init(v);

	return v;
}

void vertex_init(vertex *v)
{
	v->id = 0;
	v->state = SUSCEPTIBLE;
	v->day = 0;
}
