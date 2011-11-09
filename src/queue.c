#include "queue.h"

vertex_queue *vertex_queue_create()
{
	vertex_queue *queue = malloc(sizeof(vertex_queue));

	if(queue == NULL)
	{
		printf("Error instantiating queue\n");
	}

	vertex_queue_init(queue);

	return queue;
}

void vertex_queue_init(vertex_queue *queue)
{
	queue->head = NULL;
	queue->tail = NULL;

	queue->length = 0;
}

void vertex_queue_enqueue(vertex_queue *queue, vertex_node *node)
{
	if(queue->head == node)
	{
		printf("Trying to add node to queue that already exists in queue!\n");
	}
	else
	{
		queue->length++;

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

	queue->length--;

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

void vertex_queue_destroy(graph *g, vertex_queue *queue)
{
	vertex_node *iterator = queue->head;
	vertex_node *temp;

	while(iterator != NULL)
	{
		//printf("%p, %p\n", iterator, iterator->next);
		temp = iterator->next;
		vertex_node_pool_free(g->pool, iterator);
		iterator = temp;
	}

	queue->head = NULL;
	queue->tail = NULL;
}

_Bool vertex_queue_empty(vertex_queue *queue)
{
	if(queue->head == NULL)
	{
		return true;
	}

	return false;
}

void vertex_queue_prepend(vertex_queue *q1, vertex_queue *q2)
{
	if(q1->head != NULL && q2->head != NULL)
	{
		q2->tail->next = q1->head;
		q1->head->prev = q2->tail;
		q1->head = q2->head;

		q1->length = q1->length + q2->length;

		q1->head->backtrace = 1;
	}
}
