#include "stack.h"

vertex_stack *vertex_stack_create()
{
	vertex_stack *stack = malloc(sizeof(stack));

	vertex_stack_init(stack);
	
	return stack;
}

void vertex_stack_init(vertex_stack *stack)
{
	stack->head = NULL;
}

void vertex_stack_push(vertex_stack *stack, vertex_node *node)
{
	if(stack->head != NULL)
	{
		stack->head->prev = node;
	}

	node->prev = NULL;
	node->next = stack->head;

	stack->head = node;
}

vertex_node *vertex_stack_pop(vertex_stack *stack)
{
	if(stack->head == NULL)
	{
		return NULL;
	}
	else
	{
		vertex_node *temp = stack->head;
		stack->head = stack->head->next;
		stack->head->prev = NULL;

		return temp;
	}
}

vertex_node *vertex_stack_top(vertex_stack *stack)
{
	return stack->head;
}

bool vertex_stack_empty(vertex_stack *stack)
{
	if(stack->head == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void vertex_stack_inspect(vertex_stack *stack)
{
	vertex_node *iterator = stack->head;
	while(iterator != NULL)
	{
		printf("id: %i, address: %p, prev: %p, next: %p\n", iterator->vertex, iterator, iterator->prev, iterator->next);
		iterator = iterator->next;
	}
}
