#include "queue.h"

vertex_queue *vertex_queue_create()
{
	vertex_queue *queue = malloc(sizeof(vertex_queue));

	vertex_queue_init(queue);

	return queue;
}

void vertex_queue_init(vertex_queue *queue)
{
	queue->head = NULL;
	queue->tail = NULL;
}

void vertex_queue_enqueue(vertex_queue *queue, vertex_node *node)
{
	if(queue->head == NULL)
	{
		queue->tail = node;
	}
	else
	{
		queue->head->prev = node;
	}

	node->prev = NULL;
	node->next = queue->head;

	queue->head = node;
}

vertex_node *vertex_queue_top(vertex_queue *queue)
{
	if(queue->tail == NULL)
	{
		return NULL;
	}

	vertex_node *node = queue->tail;
}

vertex_node *vertex_queue_dequeue(vertex_queue *queue)
{
	if(queue->tail == NULL)
	{
		return NULL;
	}

	vertex_node *node = queue->tail;

	if(queue->tail->prev == NULL)
	{
		queue->head = NULL;
		queue->tail = NULL;
	}
	else
	{
		queue->tail->prev->next = NULL;
		queue->tail = queue->tail->prev;
	}

	return node;
}

void vertex_queue_inspect(vertex_queue *queue)
{
	vertex_node *iterator = queue->head;

	while(iterator != NULL)
	{
		//printf("id: %i, address: %p, prev: %p, next: %p\n", iterator->vertex->id, iterator, iterator->prev, iterator->next);
		iterator = iterator->next;
	}
}
