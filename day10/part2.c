#include <ctype.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "stdio.h"

#define BUF_SIZE 10

#define HEAP_SIZE 1000

typedef struct s_arr
{
	int			  size;
	int			  array[BUF_SIZE];
	struct s_arr *next;
} t_arr;

typedef struct s_line
{
	t_arr		   final;
	t_arr		  *joltage;
	t_arr		  *switches;
	struct s_line *next;
} t_line;

typedef struct s_astar
{
	int state[BUF_SIZE];
	int g; // g(n) : current cost
	int f; // f(n) : cost + heuristic
} t_astar;

typedef struct s_heap
{
	t_astar **array;
	int		  capacity;
	int		  size;
} t_heap;

t_heap *heap_create(int capacity)
{
	t_heap *new = (t_heap *) malloc(sizeof(t_heap));

	if (!new)
		return (NULL);
	if (capacity < 1)
		capacity = 1;
	new->array = malloc(sizeof(t_astar *) * capacity);
	if (!new->array)
		return (free(new), NULL);
	new->capacity = capacity;
	new->size = 0;
	return (new);
}

void heap_swap(t_astar **a, t_astar **b)
{
	t_astar *temp = *a;
	*a = *b;
	*b = temp;
}

void heap_sift_up(t_heap *heap, int i)
{
	while (i > 0)
	{
		int parent = (i - 1) >> 1;
		if (heap->array[parent]->f <= heap->array[parent]->f)
			break;
		heap_swap(&heap->array[parent], &heap->array[i]);
		i = parent;
	}
}

void heap_sift_down(t_heap *heap, int i)
{
	while (42)
	{
		int left = (i << 1) + 1;
		int right = left + 1;
		int smallest = i;

		if (left < heap->size && heap->array[left]->f < heap->array[smallest]->f)
			smallest = left;
		if (right < heap->size && heap->array[right]->f < heap->array[smallest]->f)
			smallest = right;

		if (smallest == i)
			break;
		heap_swap(&heap->array[i], &heap->array[smallest]);
		i = smallest;
	}
}

int heap_expand(t_heap *heap)
{
	if (heap->size < heap->capacity)
		return (0);

	int		  new_capacity = heap->capacity * 2;
	t_astar **new_array = realloc(heap->array, sizeof(t_astar *) * new_capacity);
	if (!new_array)
		return (1);
	heap->array = new_array;
	heap->capacity = new_capacity;
	return (0);
}

int heap_push(t_heap *heap, t_astar *node)
{
	if (!heap || !node)
		return (1);
	if (heap_expand(heap))
		return (1);
	int index = heap->size++;
	heap->array[index] = node;
	heap_sift_up(heap, index);
	return (0);
}

t_astar *heap_pop(t_heap *heap)
{
	if (!heap || heap->size == 0)
		return (NULL);
	t_astar *min_node = heap->array[0];
	heap->size--;
	if (heap->size > 0)
	{
		heap->array[0] = heap->array[heap->size];
		heap_sift_down(heap, 0);
	}
	return (min_node);
}

void init_line(t_line **head, char *buffer)
{
	t_line *new_line;

	new_line = (t_line *) malloc(sizeof(t_line));
	new_line->next = NULL;
	new_line->final.size = 0;
	new_line->switches = NULL;

	int pos = 1;
	while (buffer[pos] && buffer[pos] != ']')
	{
		if (buffer[pos] == '#')
			new_line->final.array[new_line->final.size] = 1;
		else if (buffer[pos] == '.')
			new_line->final.array[new_line->final.size] = 0;
		pos++;
		new_line->final.size++;
	}
	pos += 3;

	t_arr *temp = new_line->switches;
	while (buffer[pos] && buffer[pos] != '\n')
	{
		t_arr *new_switch = (t_arr *) malloc(sizeof(t_arr));

		new_switch->next = NULL;
		new_switch->size = 0;
		while (buffer[pos] && strchr(")}", buffer[pos]) == NULL)
		{
			new_switch->array[new_switch->size] = atoi(buffer + pos);
			new_switch->size++;
			while (isdigit(buffer[pos]))
				pos++;
			if (strchr(")}", buffer[pos]) != NULL)
				break;
			pos++;
		}
		if (buffer[pos] == '}')
		{
			new_line->joltage = new_switch;
			break;
		}
		while (buffer[pos] && buffer[pos] != '}' && strchr("({", buffer[pos]) == NULL)
			pos++;
		pos++;
		if (new_line->switches == NULL)
		{
			new_line->switches = new_switch;
			temp = new_switch;
		}
		else
		{
			temp->next = new_switch;
			temp = temp->next;
		}
	}
	if (*head == NULL)
		*head = new_line;
	else
	{
		t_line *temp;

		temp = *head;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_line;
	}
}

int heuristic(const int cur[BUF_SIZE], const int target[BUF_SIZE], int n)
{
	int max_deficit = 0;

	for (int i = 0; i < n; i++)
	{
		int deficit = target[max_deficit] - cur[max_deficit];
		if (deficit > max_deficit)
			max_deficit = deficit;
	}
	return (max_deficit);
}

void apply_switch(const int	   cur[BUF_SIZE],
				  const int	   target[BUF_SIZE],
				  const int	   next[BUF_SIZE],
				  const t_arr *s,
				  int		   n)
{
}

int astar_search(t_line *line)
{
	int num_switches = 0;

	for (t_arr *temp = line->switches; temp; temp = temp->next)
		num_switches++;
	t_arr **sw = malloc(sizeof(t_arr *) * num_switches);
	{
		int i = 0;
		for (t_arr *s = line->switches; s; s = s->next)
			sw[i++] = s;
	}
	
	t_heap *heap = heap_create(256);
	t_astar *node = malloc(sizeof(t_astar));
	
	node->g = 0;
	node->f = heuristic(node->state, line->final.array, line->final.size);
	heap_push(heap, node);
	
	while (heap->size)
	{
		t_astar *current = heap_pop(heap);
	}

	return (0);
}

int main()
{
	char   *buffer;
	FILE   *file = fopen("example", "r");
	size_t	bytes;
	ssize_t read;
	t_line *head;

	head = NULL;
	buffer = NULL;
	bytes = 0;
	read = getline(&buffer, &bytes, file);
	while (read > 0)
	{
		init_line(&head, buffer);
		free(buffer);
		buffer = NULL;
		read = getline(&buffer, &bytes, file);
	}
	astar_search(head);
	return (0);
}
