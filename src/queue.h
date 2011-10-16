#ifndef VERTEX_QUEUE_HEADER
#define VERTEX_QUEUE_HEADER

struct vertex_queue_struct
{
	struct vertex_node_struct *head;
	struct vertex_node_struct *tail;

	int length;
};

typedef struct vertex_queue_struct vertex_queue;

vertex_queue *vertex_queue_create();
void vertex_queue_init(vertex_queue *);

void vertex_queue_enqueue(vertex_queue *, vertex_node *);
vertex_node *vertex_queue_top(vertex_queue *queue);
vertex_node *vertex_queue_dequeue(vertex_queue *queue);


void vertex_queue_inspect(vertex_queue *);

#endif
