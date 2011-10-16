#ifndef VERTEX_HEADER
#define VERTEX_HEADER

enum state {SUSCEPTIBLE, LATENT, INFECTIOUS, RECOVERED };
typedef enum state state;

struct vertex
{
	int id;
	int day;
	state state;
};

typedef struct vertex vertex;

vertex *vertex_create();
void vertex_init();

#endif
