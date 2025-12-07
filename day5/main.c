#include <stddef.h>
#include <string.h>

#include "stdio.h"
#include "stdlib.h"

typedef struct s_range
{
	long long min;
	long long max;
} t_range;

int		comp(const void *a, const void *b);
int		get_ranges(t_range ranges[200], FILE *file);
int		check_ingredient(t_range ranges2[200], char *buffer, int count);
t_range lst_new(char *buffer);
int		remove_overlap(t_range old[200], t_range new[200], int size);

int main()
{
	t_range orig_ranges[200];
	int		old_size;
	FILE   *file = fopen("input", "r");

	old_size = get_ranges(orig_ranges, file);

	t_range new_ranges[200];
	int		new_size = remove_overlap(orig_ranges, new_ranges, old_size);

	char   *buffer = NULL;
	size_t	bytes = 0;
	int		fresh = 0;
	ssize_t read = getline(&buffer, &bytes, file);
	while (read > 0)
	{
		buffer[read - 1] = '\0';
		fresh += check_ingredient(new_ranges, buffer, new_size);
		free(buffer);
		buffer = NULL;
		bytes = 0;
		read = getline(&buffer, &bytes, file);
	}
	free(buffer);

	unsigned long long result = 0;
	for (int i = 0; i < new_size; i++)
		result += new_ranges[i].max - new_ranges[i].min;

	printf("Fresh ingredients: %i\n", fresh);
	printf("Total fresh ID ranges: %lli\n", result);

	fclose(file);
	return (0);
}

// parse file for ID ranges, store info in parameter ranges
// returns number of ranges given in file
int get_ranges(t_range ranges[200], FILE *file)
{
	char   *buffer = NULL;
	size_t	bytes = 0;
	ssize_t read = 0;
	int		count = 0;

	read = getline(&buffer, &bytes, file);
	while (read > 0 && buffer[0] != '\n')
	{
		ranges[count] = lst_new(buffer);
		free(buffer);
		buffer = NULL;
		bytes = 0;
		read = getline(&buffer, &bytes, file);
		count++;
	}
	qsort(ranges, count, sizeof(t_range), comp);
	free(buffer);
	return (count);
}

int remove_overlap(t_range orig[200], t_range new[200], int size)
{
	int new_size;
	int i;
	int j;

	new_size = 0;
	i = 0;
	while (i < size)
	{
		t_range range = orig[i];
		for (j = i + 1; (j < size) && (range.max > orig[j].min); j++)
		{
			if (orig[j].max > range.max)
				range.max = orig[j].max;
		}
		new[new_size] = range;
		new_size++;
		i = j;
	}
	return (new_size);
}

t_range lst_new(char *buffer)
{
	t_range new;
	int		dash_pos;

	dash_pos = 0;
	while (buffer[dash_pos] != '-')
		dash_pos++;

	new.min = atoll(buffer);
	new.max = atoll(buffer + dash_pos + 1) + 1;
	// printf("min = %lli, max = %lli\n", new->min, new->max);
	return (new);
}

int check_ingredient(t_range ranges[200], char *buffer, int size)
{
	long long num = atoll(buffer);

	for (int i = 0; i < size; i++)
		if (ranges[i].min <= num && ranges[i].max >= num)
			return (1);
	return (0);
}

int comp(const void *a, const void *b)
{
	t_range *left = (t_range *) a;
	t_range *right = (t_range *) b;

	return (left->min > right->min);
}
