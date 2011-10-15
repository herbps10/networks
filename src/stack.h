#ifndef VERTEX_STACK_HEADER
#define VERTEX_STACK_HEADER

#define false 0
#define true 1

typedef _Bool bool;

struct vertex_stack
{
	struct vertex_node *head;
};

typedef struct vertex_stack vertex_stack;

vertex_stack *vertex_stack_create();
void vertex_stack_init(vertex_stack *);

void vertex_stack_push(vertex_stack *, vertex_node *);
vertex_node *vertex_stack_pop(vertex_stack *);
vertex_node *vertex_stack_top(vertex_stack *);
bool vertex_stack_empty(vertex_stack *);

#endif
