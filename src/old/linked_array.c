struct node *node_create()
{
	node *node = malloc(sizeof node);
	node->next = -1;
	node->prev = -1;
}

void linked_array_init(linked_array *array)
{
	head = -1;
	tail = -1;
	for(int i = 0; i < NETWORK_SIZE; i++)
	{
		array->items[0] = node_create();
	}
}
