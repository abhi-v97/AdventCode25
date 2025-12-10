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
	t_arr		  *switches;
	struct s_line *next;
} t_line;

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
	while (buffer[pos])
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

int main()
{
	char   *buffer;
	FILE   *file = fopen("example", "r");
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
	return (0);
}
