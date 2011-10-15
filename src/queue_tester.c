#include "stdio.h"
#include "stdlib.h"

#include "vertex_node.c"
#include "queue.c"

int main()
{
	vertex_node *node1 = vertex_node_create();
	node1->vertex = 1;

	vertex_node *node2 = vertex_node_create();
	node2->vertex = 2;

	vertex_node *node3 = vertex_node_create();
	node3->vertex = 3;


	vertex_queue *queue = vertex_queue_create();

	vertex_queue_enqueue(queue, node1);
	vertex_queue_enqueue(queue, node2);
	vertex_queue_enqueue(queue, node3);

	printf("Total queue, expected: 3 2 1\n");
	vertex_queue_inspect(queue);
	printf("\n\n");

	printf("Dequeue, expected: 1\n");
	vertex_node *n = vertex_queue_dequeue(queue);
	printf("%i\n", n->vertex);

	printf("Total queue: expected 3 2\n");
	vertex_queue_inspect(queue);
	printf("\n\n");

	printf("Dequeue, expected: 2\n");
	n = vertex_queue_dequeue(queue);
	printf("%i\n", n->vertex);

	printf("Total queue: expected 3 2\n");
	vertex_queue_inspect(queue);
	printf("\n\n");

	return 0;
}
