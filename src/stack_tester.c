#include "stdio.h"
#include "stdlib.h"

#include "vertex_node.c"
#include "stack.c"

int main()
{
	vertex_node *node1 = vertex_node_create();
	node1->vertex = 1;

	vertex_node *node2 = vertex_node_create();
	node2->vertex = 2;

	vertex_node *node3 = vertex_node_create();
	node3->vertex = 3;


	vertex_stack *stack = vertex_stack_create();

	vertex_stack_push(stack, node1);
	vertex_stack_push(stack, node2);
	vertex_stack_push(stack, node3);

	printf("Total stack, expected: 3 2 1\n");
	vertex_stack_inspect(stack);
	printf("\n\n");

	printf("Pop, expected: 3\n");
	vertex_node *n = vertex_stack_pop(stack);
	printf("%i\n", n->vertex);

	printf("Total stack: expected 2 1\n");
	vertex_stack_inspect(stack);
	printf("\n\n");

	printf("Pop, expected: 2\n");
	n = vertex_stack_pop(stack);
	printf("%i\n", n->vertex);

	printf("Total stack: expected 1\n");
	vertex_stack_inspect(stack);
	printf("\n\n");

	return 0;
}
