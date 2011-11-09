#include "stack.h"

vertex_stack *vertex_stack_create()
{
	vertex_stack *stack = malloc(sizeof(stack));

	if(stack == NULL)
	{
		printf("Cannot instantiate stack\n");
	}

	vertex_stack_init(stack);
	
	return stack;
}

void vertex_stack_init(vertex_stack *stack)
{
	stack->head = NULL;
}

void vertex_stack_push(struct vertex_stack_struct *stack, struct vertex_node_struct *node)
{
	if(stack->head != NULL)
	{
		stack->head->prev = node;
	}

	node->prev = NULL;
	node->next = stack->head;

	stack->head = node;
}

vertex_node *vertex_stack_pop(struct vertex_stack_struct *stack)
{
	if(stack->head == NULL)
	{
		return NULL;
	}
	else
	{
		vertex_node *temp = stack->head;
		stack->head = stack->head->next;

		if(stack->head != NULL)
		{
			stack->head->prev = NULL;
		}

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
		//printf("id: %i, address: %p, prev: %p, next: %p\n", iterator->vertex->id, iterator, iterator->prev, iterator->next);
		iterator = iterator->next;
	}
}

void vertex_stack_destroy(graph *g, vertex_stack *stack)
{
	vertex_node *node;
	while(vertex_stack_top(stack) != NULL)
	{
		node = vertex_stack_pop(stack);
		node->vertex = NULL;
		vertex_node_pool_free(g->pool, node);
	}

	stack->head = NULL;
}
