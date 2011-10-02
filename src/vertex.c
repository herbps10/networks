/**
 * Creates a vertex
 */
vertex vertex_create()
{
	vertex v;
	vertex_init(&v);

	return v;
}

/**
 * Initializes a vertex
 */
void vertex_init(vertex *v)
{
	v->state_day = 0;
	v->state = SUSCEPTIBLE;
	v->first_neighbor = NULL;
	v->degree = 0; // Doesn't have any neighbors so degree is 0
}

/**
 * Frees all memory related to vertex
 * 
 * Walks the linked list and frees each node
 */
void vertex_destroy(vertex *v)
{
	vertex_list_destroy(v->first_neighbor);
}

/**
 * Adds an edge between two vertices
 *
 * Takes two vertices
 * Adds a link from v --> neighbor and from neighbor --> v
 * 
 * Time complexity: O(1)
 */
void vertex_add_edge(vertex *v, vertex *neighbor) {
	vertex_add_adjacency(v, neighbor);
	vertex_add_adjacency(neighbor, v);
}

/**
 * Adds a link to neighbor in v
 *
 * This indicates that v is adjacent to neighbor
 * Does not modify neighbor, so this creates a
 * one directional edge from v --> vertex.
 */
void vertex_add_adjacency(vertex *v, vertex *neighbor)
{
	// Allocate space for the new node that will
	// go on v's adjacency linked list
	vertex_list_node *node;
	node = malloc(sizeof(vertex_list_node));
	
	node->vertex = neighbor;
	node->next = v->first_neighbor;
	node->prev = NULL;

	if(v->first_neighbor != NULL)
	{
		v->first_neighbor->prev = node;
	}

	v->first_neighbor = node;

	v->degree++;
}

/**
 * Deletes an edge between two vertices
 *
 * Time complexity: O( |E| )
 */
void vertex_delete_edge(vertex *v, vertex *neighbor)
{
	vertex_delete_adjacency(v, neighbor);
	vertex_delete_adjacency(neighbor, v);
}

void vertex_delete_adjacency(vertex *v, vertex *neighbor)
{
	vertex_list_node *iterator = v->first_neighbor;

	while(iterator != NULL)
	{
		if(iterator->vertex == neighbor)
		{
			// Remove the current vertex from the linked list
			if(iterator->prev == NULL)
			{
				v->first_neighbor = iterator->next;
			}
			else
			{
				iterator->prev->next = iterator->next;	
			}

			if(iterator->next != NULL)
			{
				iterator->next->prev = NULL;
				iterator->next->prev = iterator->prev;
			}

			free(iterator);

			v->degree--; // Since we're deleting an edge, the degree goes down by one

			return;
		}
		iterator = iterator->next;
	}
}

/**
 * Searches the adjacent vertices to haystack looking for needle
 */
short vertex_edge_exists(vertex *haystack, vertex *needle)
{
	return vertex_list_contains(haystack->first_neighbor, needle);
}

/**
 * Change the disease state of the vertex
 */
void vertex_set_state(vertex *v, state s, int day)
{
	v->state = s;
	v->state_day = day;
}

/**
 * Tries to spread the infection to its neighbors
 * 
 * Returns a linked list of any newly infected vertices
 */
vertex_list_node *vertex_infect_neighbors(vertex *v, int day)
{
	vertex_list_node *iterator, *new_latent = NULL;

	// The vertex can only infect others if it is infectious
	// Also if it was just set to the infectious state this turn then it can't infect anyone else
	if(v->state == INFECTIOUS && day - v->state_day > 0)
	{
		// Go through each neighbor
		iterator = v->first_neighbor;

		while(iterator != NULL)
		{
			// If the vertex is recovered, then there is a chance
			// that it is ready to move back into the susceptible class.
			//
			// Check to see if it should be moved back into the susceptible class.
			if(iterator->vertex->state == RECOVERED)
			{
				if(day - iterator->vertex->state_day > DAYS_RECOVERED)
				{
					vertex_set_state(iterator->vertex, SUSCEPTIBLE, day);
				}
			}
			
			// If the neighbor is susceptible, try to infect it!
			if(iterator->vertex->state == SUSCEPTIBLE)
			{
				// If the disease was spread to the neighbor, 
				// then add it to the list of newly infected vertices
				if(vertex_spread_infection(v, iterator->vertex, day) == TRUE)
				{
					new_latent = vertex_list_append(new_latent, iterator->vertex);
				}
			}
			

			iterator = iterator->next;
		}
	}

	return new_latent;
}

/**
 * Attempt to spread infection from source to target by probability T
 *
 * Does not check to make sure that vertex target is not already infected, latent, or recovered
 */
_Bool vertex_spread_infection(vertex *source, vertex *target, int day)
{
	double T, r;

	// The probability of infecting a neighbor is proportional to
	// how the degree of the vertex. This is so that the number of
	// of secondary infections caused by the vertex will roughly equal
	// R0.
	T = 1 - (1 - (((double)R0/(double)source->degree) / (double)DAYS_INFECTIOUS));

	// Generate a random number between 0 and 1
	// If it is smaller than T, then spread the infection
	r = (double)rand() / (double)RAND_MAX;

	// If we infec the neighbor, put them into the
	// susceptible class
	if(r < T)
	{
		vertex_set_state(target, LATENT, day);

		return TRUE;
	}

	return FALSE;
}
