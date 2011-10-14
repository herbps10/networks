#ifndef VERTEX_LIST_HEADER_GUARD
#define VERTEX_LIST_HEADER_GUARD

#include "linked_array.c"

/**
 * Linked Array struct
 */

typedef struct node node;

struct node
{
	vertex *v;
	int next;
	int prev;
}

struct node *node_create();

typedef struct linked_array linked_array;
struct linked_array
{
	struct node *items[NETWORK_SIZE];
	int head;
	int tail;
};

void linked_array_init(linked_array *);
