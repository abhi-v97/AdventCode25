#include <stddef.h>
#include <string.h>

#include "stdio.h"
#include "stdlib.h"

typedef struct s_bigint
{
	char			*min;
	char			*max;
	int				 certified_fresh;
	struct s_bigint *next;
} t_bigint;

void lst_addnew(t_bigint **head, t_bigint *new)
{
	t_bigint *temp = NULL;

	if (head == NULL)
		return;

	if (*head == NULL)
	{
		*head = new;
	}
	else
	{
		temp = *head;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
	}
}

t_bigint *lst_newnode(char *buffer)
{
	t_bigint *new;

	new = (t_bigint *) malloc(sizeof(t_bigint));

	int min_size = 0;
	while (buffer[min_size] != '-')
		min_size++;

	new->min = (char *) malloc((min_size + 1) * sizeof(char));
	for (int i = 0; i < min_size; i++)
		new->min[i] = buffer[i];

	int max_size = 0;
	while (buffer[min_size + max_size + 1] && buffer[min_size + max_size + 1] != '\n')
		max_size++;

	new->max = (char *) malloc((max_size + 1) * sizeof(char));
	for (int i = 0; i < max_size; i++)
		new->max[i] = buffer[i + min_size + 1];

	new->min[min_size] = '\0';
	new->max[max_size] = '\0';
	new->next = NULL;
	new->certified_fresh = 0;
	// printf("min = %s, max = %s\n", new->min, new->max);
	return (new);
}

void create_dict(t_bigint **head, FILE *file)
{
	char   *buffer = NULL;
	size_t	bytes = 0;
	ssize_t read = 0;

	read = getline(&buffer, &bytes, file);
	while (read > 0 && buffer[0] != '\n')
	{
		lst_addnew(head, lst_newnode(buffer));
		free(buffer);
		buffer = NULL;
		bytes = 0;
		read = getline(&buffer, &bytes, file);
	}
	free(buffer);
}

int is_above_min(char *min, char *buffer)
{
	int min_size = strlen(min);
	int buf_size = strlen(buffer) - 1;

	if (buf_size > min_size)
		return (1);
	else if (buf_size == min_size)
	{
		int i = 0;
		while (buffer[i + 1] != '\n' && buffer[i] == min[i])
			i++;
		if (buffer[i] >= min[i])
			return (1);
		else
			return (0);
	}
	return (0);
}

int is_below_max(char *max, char *buffer)
{
	int max_size = strlen(max);
	int buf_size = strlen(buffer) - 1;

	if (buf_size > max_size)
		return (0);
	else if (buf_size == max_size)
	{
		int i = 0;
		while (buffer[i + 1] != '\n' && buffer[i] == max[i])
			i++;
		if (buffer[i] <= max[i])
			return (1);
		else
			return (0);
	}
	return (0);
}

int compare_ranges(t_bigint **head, char *buffer)
{
	t_bigint *temp = *head;

	while (temp != NULL)
	{
		if (is_above_min(temp->min, buffer) && is_below_max(temp->max, buffer))
		{
			temp->certified_fresh = 1;
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

int subtract(char *min, char *max)
{
	int result = 0;
	
	return (result);
}

int main()
{
	t_bigint **list;
	FILE	  *file = fopen("input", "r");

	list = (t_bigint **) malloc(sizeof(t_bigint *));
	*list = NULL;
	create_dict(list, file);

	t_bigint *temp = *list;
	char	 *buffer = NULL;
	size_t	  bytes = 0;
	ssize_t	  read = 0;

	read = getline(&buffer, &bytes, file);
	int fresh = 0;
	int count = 0;
	while (read > 0)
	{
		if (compare_ranges(list, buffer))
			fresh++;

		free(buffer);
		buffer = NULL;
		bytes = 0;
		read = getline(&buffer, &bytes, file);
		count++;
	}
	
	count = 0;
	for (t_bigint *temp = *list; temp != NULL; temp = temp->next)
	{
		if (temp->certified_fresh == 1)
			count++;
	}
	printf("S = %i, i = %i\n", fresh, count);
	fclose(file);
	return (0);
}
