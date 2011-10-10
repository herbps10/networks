/**
 * Walks a vertex list freeing nodes as it goes
 *
 * Time Complexity: O(|E|)
 */
void vertex_list_destroy(vertex_list_node *list)
{
	vertex_list_node *iterator = list, *temp;

	while(iterator != NULL)
	{
		temp = iterator;
		iterator = iterator->next;

		free(temp);
	}
}

/**
 * Walks a vertex list freeing all the vertex structs on the list
 *
 * Time Complexity: O(|E|)
 */
void vertex_list_destroy_vertices(vertex_list_node *list)
{
	vertex_list_node *iterator = list;

	while(iterator != NULL)
	{
		free(iterator->vertex);
		iterator = iterator->next;
	}
}

/**
 * Searches list looking for presence of a vertex
 *
 * Time Complexity: O(|E|)
 */
int vertex_list_contains(vertex_list_node *list, vertex *v)
{
	vertex_list_node *iterator = list;

	while(iterator != NULL)
	{
		if(iterator->vertex == v)
		{
			return TRUE;
		}

		iterator = iterator->next;
	}

	return FALSE;
}

/**
 * Appends a vertex to a linked list of vertices
 */
vertex_list_node *vertex_list_append(vertex_list_node *list, vertex *v)
{
	vertex_list_node *node = malloc(sizeof(vertex_list_node));

	node->vertex = v;
	node->prev = NULL;
	node->next = list;

	if(list != NULL)
	{
		list->prev = node;
	}
	
	return node;
}

/**
 * Combines two lists together
 * list2 will be appended to list1
 */
vertex_list_node *vertex_list_merge(vertex_list_node *list1, vertex_list_node *list2)
{
	if(list1 == NULL)
	{
		return list2;
	}
	else if(list2 == NULL)
	{
		return list1;
	}
	else 
	{
		vertex_list_node *iterator = list1;
		while(iterator->next != NULL)
		{
			iterator = iterator->next;
		}

		iterator->next = list2;
		list2->prev = iterator;

		return list1;
	}
}

/**
 * Takes the tail of one list and has it continue to the next
 */
void vertex_list_combine(vertex_list_node *tail, vertex_list_node *head)
{
	if(tail != NULL)
	{
		tail->next = head;
		head->prev = tail;
	}
}

/**
 * Prints the list
 */
void vertex_list_inspect(vertex_list_node *list1)
{
	vertex_list_node *iterator = list1;

	while(iterator != NULL)
	{
		printf("Node: %p. Previous: %p, Next: %p\n", iterator, iterator->prev, iterator->next);

		iterator = iterator->next;
	}
}
