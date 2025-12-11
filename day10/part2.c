#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "stdio.h"

#define BUF_SIZE 10

#define HT_SIZE 32768

typedef struct s_arr
{
	int			  size;
	int			  array[BUF_SIZE];
	struct s_arr *next;
} t_arr;

typedef struct s_line
{
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

// hash table, similar to python set
typedef struct s_hash
{
	char		  *key;
	int			   g;
	struct s_hash *next;
} t_hash;

static t_hash *ht[HT_SIZE];

uint32_t djb2(const char *s)
{
	uint32_t h = 5381;

	while (*s)
	{
		// equivalent to h = h * 33 + *s;
		h = ((h << 5) + h) + *s++;
	}
	return (h);
}

static int ht_get(const char *key)
{
	uint32_t idx = djb2(key) & (HT_SIZE - 1);
	for (t_hash *e = ht[idx]; e != NULL; e = e->next)
	{
		if (strcmp(e->key, key) == 0)
			return (e->g);
	}
	return (-1);
}

static void ht_put(const char *key, int g)
{
	uint32_t idx = djb2(key) & (HT_SIZE - 1);
	for (t_hash *e = ht[idx]; e != NULL; e = e->next)
	{
		if (strcmp(e->key, key) == 0)
		{
			e->g = g;
			return;
		}
	}
	t_hash *new = malloc(sizeof(t_hash));
	new->key = strdup(key);
	new->g = g;
	new->next = ht[idx];
	ht[idx] = new;
}

void ht_clear(void)
{
	for (int i = 0; i < HT_SIZE; i++)
	{
		t_hash *e = ht[i];

		while (e)
		{
			t_hash *new = e->next;
			free(e->key);
			free(e);
			e = new;
		}
		ht[i] = NULL;
	}
}

// uint32_t idx = djb2(key) & (HT_SIZE - 1);

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
		if (heap->array[parent]->f <= heap->array[i]->f)
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

void heap_free(t_heap *heap)
{
	for (int i = 0; i < heap->size; i++)
		free(heap->array[i]);
	free(heap->array);
	free(heap);
}

void init_line(t_line **head, char *buffer)
{
	t_line *new_line;

	new_line = (t_line *) malloc(sizeof(t_line));
	new_line->next = NULL;
	new_line->switches = NULL;

	int pos = 1;
	while (buffer[pos] && buffer[pos] != ']')
		pos++;
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
		int deficit = target[i] - cur[i];
		if (deficit > max_deficit)
			max_deficit = deficit;
	}
	return (max_deficit);
}

void apply_switch(
	const int cur[BUF_SIZE], int next[BUF_SIZE], const int target[BUF_SIZE], const t_arr *s, int n)
{
	for (int i = 0; i < n; i++)
		next[i] = cur[i];

	for (int j = 0; j < s->size; j++)
	{
		next[s->array[j]] = next[s->array[j]] + 1;
		// int index = s->array[j];
		// if (index >= 0 && index < n)
		// {
		// 	next[index]++;
		// }
	}
}

void serialise_state(const int state[BUF_SIZE], int n, char *buf, int bufsize)
{
	int offset = 0;

	for (int i = 0; i < n; i++)
	{
		int size = 0;
		if (bufsize > offset)
			size = bufsize - offset;
		offset += snprintf(buf + offset, size, "%d ", state[i]);
	}
}

int astar_search(t_line *line, int target[BUF_SIZE])
{
	int num_buttons = 0;
	int n = line->joltage->size;

	for (t_arr *temp = line->switches; temp; temp = temp->next)
		num_buttons++;
	t_arr **switches = malloc(sizeof(t_arr *) * num_buttons);
	{
		int i = 0;
		for (t_arr *s = line->switches; s; s = s->next)
			switches[i++] = s;
	}

	for (int i = 0; i < HT_SIZE; i++)
		ht[i] = NULL;

	t_heap	*heap = heap_create(256);
	t_astar *start = malloc(sizeof(t_astar));

	char test[BUF_SIZE * 5];
	int	 init_state[BUF_SIZE] = {0};

	serialise_state(init_state, n, test, sizeof(test));
	ht_put(test, 0);

	for (int i = 0; i < n; i++)
		start->state[i] = init_state[i];
	start->g = 0;
	start->f = heuristic(start->state, target, line->joltage->size);
	heap_push(heap, start);

	while (heap->size)
	{
		t_astar *current = heap_pop(heap);

		serialise_state(current->state, n, test, sizeof(test));
		int best_known = ht_get(test);
		// check if current node surpasses best known g(n)
		if (best_known >= 0 && current->g > best_known)
		{
			free(current);
			continue;
		}

		int done = 1;
		for (int i = 0; i < n; i++)
		{
			if (current->state[i] != target[i])
			{
				done = 0;
				break;
			}
		}
		if (done)
		{
			// FREEDOM
			int result = current->g;
			free(current);
			heap_free(heap);
			free(switches);
			ht_clear();
			return (result);
		}

		for (int index = 0; index < num_buttons; index++)
		{
			int next_state[BUF_SIZE];

			apply_switch(current->state, next_state, target, switches[index], n);
			int g2 = current->g + 1;

			serialise_state(next_state, n, test, sizeof(test));
			int prevg = ht_get(test);
			if (prevg >= 0 && prevg <= g2)
				continue;

			ht_put(test, g2);
			t_astar *new = malloc(sizeof(t_astar));
			for (int i = 0; i < n; i++)
				new->state[i] = next_state[i];
			new->g = g2;
			new->f = g2 + heuristic(next_state, target, n);
			heap_push(heap, new);
		}
		free(current);
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
	int result = 0;
	for (t_line *temp = head; temp != NULL; temp = temp->next)
	{
		result += astar_search(temp, temp->joltage->array);
		printf("res = %i\n", result);
	}
	return (0);
}
