/**
 * Searches list looking for presence of a vertex
 *
 * Time Complexity: O(|E|)
 */
int vertex_list_contains(vertex_list *list, vertex *v)
{
	for(int i = list->start; i < list->end; i++)
	{
		if(*list[i]->vertex == v)
		{
			return TRUE;
		}
	}

	return FALSE;
}

/**
 * Copies the elements in the second list to the end of the first
 */
void vertex_list_append(vertex_list *list1, vertex_list *list2)
{
	for(int i = list2->start; i < list2->end, i++)
	{
		vertex_list_add(list1, list2[i]);
	}
}

/**
 * Adds a vertex to the list
 */
void vertex_list_add(vertex_list *list, vertex *v)
{
	list[list->end] = vertex;

	list->end++;

	if(list->end == NETWORK_SIZE)
	{
		list->end = 0;
	}
}

/**
 * Calculates the length of the list
 */
int vertex_list_length(vertex_list *list)
{
	if(list->start < list->end)
	{
		return list->end - list->start;
	}
	else
	{
		return (NETWORK_SIZE - list->start) + list->end;
	}
}
