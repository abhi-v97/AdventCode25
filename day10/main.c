#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "stdio.h"

#define BUF_SIZE 10
#define PROBLEMS 3

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

typedef struct s_qnode
{
	struct s_qnode *next;
	int				buttons_pressed;
	int				state[10];
} t_qnode;

typedef struct s_queue
{
	t_qnode *head;
	t_qnode *tail;
} t_queue;

// mallocs and initialises an empty queue
// returns pointer to stack on success, NULL on failure
t_queue *init_queue()
{
	t_queue *new;

	new = (t_queue *) malloc(sizeof(t_queue));
	if (!new)
		return (NULL);
	new->head = NULL;
	new->tail = NULL;
	return (new);
}

// adds value as a new node in the queue
// returns 0 on success, 1 on failure
int enqueue(t_queue *q, int test[10], int buttons_pressed)
{
	t_qnode *new;

	new = (t_qnode *) malloc(sizeof(t_qnode));
	if (!new)
		return (1);
	memcpy(new->state, test, sizeof(new->state));
	if (!q->tail)
	{
		q->tail = new;
		q->head = new;
	}
	else
	{
		q->tail->next = new;
		q->tail = new;
	}
	new->buttons_pressed = buttons_pressed;
	return (0);
}

// removes an element from the queue
// updates *value with the removed element
// returns 0 on success, 1 on failure
int dequeue(t_queue *q)
{
	t_qnode *temp;

	if (!q)
		return (1);
	temp = q->head;
	q->head = temp->next;
	free(temp);
	return (0);
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
	for (t_arr *temp = new_line->switches; temp->next != NULL; temp = temp->next)
		;
	new_line->joltage = temp;
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

int is_answer(t_line *head, int *arr)
{
	for (int i = 0; i < head->final.size; i++)
	{
		if (head->final.array[i] != arr[i])
			return (0);
	}
	return (1);
}

void press_buttons(int test[10], t_arr *arr)
{
	for (int i = 0; i < arr->size; i++)
	{
		test[arr->array[i]] = !test[arr->array[i]];
	}
}

int bfs(t_line *head)
{
	t_queue *queue = init_queue();

	// root
	for (t_arr *temp = head->switches; temp->next != NULL; temp = temp->next)
	{
		int test[10] = {0};
		press_buttons(test, temp);
		if (is_answer(head, test))
			return (1);
		enqueue(queue, test, 1);
	}

	while (queue->head)
	{
		int test[10];

		for (t_arr *temp = head->switches; temp->next != NULL; temp = temp->next)
		{
			memcpy(test, queue->head->state, sizeof(test));
			press_buttons(test, temp);
			if (is_answer(head, test))
				return (queue->head->buttons_pressed + 1);
			enqueue(queue, test, queue->head->buttons_pressed + 1);
		}
		dequeue(queue);
	}
	return (0);
}

// int recursive_search(t_line *line)
// {
// 	for (t_arr *temp = line->switches; temp->next != NULL; temp = temp->next)
// 	{
// 	}
// 	return (0);
// }

int main()
{
	char   *buffer;
	FILE   *file = fopen("input", "r");
	int		headaches;
	size_t	bytes;
	ssize_t read;
	t_line *head;

	head = NULL;
	headaches = 0;
	buffer = NULL;
	bytes = 0;
	read = getline(&buffer, &bytes, file);
	while (read > 0)
	{
		init_line(&head, buffer);
		free(buffer);
		buffer = NULL;
		read = getline(&buffer, &bytes, file);
		headaches++;
	}

	int result = 0;
	for (t_line *temp = head; temp != NULL; temp = temp->next)
	{
		result += bfs(temp);
		printf("res: %i\n", result);
	}
	// recursive_search(head);
	return (0);
}
