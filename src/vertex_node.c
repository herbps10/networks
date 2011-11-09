#include "vertex_node.h"

vertex_node *vertex_node_create()
{
	vertex_node *node = malloc(sizeof(vertex_node));

	if(node == NULL)
	{
		printf("Error instantiating node\n");
	}

	vertex_node_init(node);

	return node;
}

void vertex_node_init(vertex_node *node)
{
	node->next = NULL;
	node->prev = NULL;
	node->vertex = 0;
}
