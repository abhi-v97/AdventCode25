#include "stdio.h"

#define SIZE 8

typedef struct s_pos
{
	int x;
	int y;
} t_pos;

int comp(const void *a, const void *b)
{
	t_pos *aa = (t_pos *) a;
	t_pos *bb = (t_pos *) b;

	return (aa->x < bb->x);
}

int check_valid(int k, int i, int j)
{
	if (j > i)
		return (check_valid(k, j, i));

	if (k > i && k < j)
		return (1);
	return (0);
}

int check_green(t_pos pos[SIZE], t_pos kpos)
{
	for (int i = 0; i < SIZE; i++)
	{
	}
	return (0);
}

int main()
{
	FILE *file;

	file = fopen("example", "r");
	char buffer[30];

	t_pos pos[SIZE];

	int i = 0;
	while (fgets(buffer, sizeof(buffer), file) != NULL)
	{
		sscanf(buffer, "%i,%i\n", &pos[i].x, &pos[i].y);
		i++;
	}

	t_pos pos1;
	t_pos pos2;
	long  max_size = 0;
	int	  invalid;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (i == j)
				continue;
			invalid = 0;
			for (int k = 0; k < SIZE; k++)
			{
				if (pos[k].x == pos[i].x && pos[k].y == pos[i].y)
					continue;
				else if (pos[k].x == pos[j].x && pos[k].y == pos[j].y)
					continue;
				if (check_valid(pos[k].x, pos[i].x, pos[j].x) ||
					check_valid(pos[k].y, pos[i].y, pos[j].y))
				{
					invalid = 1;
					break;
				}
			}
			if (invalid)
				continue;
			long xx = (pos[i].x - pos[j].x) + 1;
			long yy = (pos[i].y - pos[j].y) + 1;
			long temp_size = xx * yy;
			if (temp_size > max_size)
				max_size = temp_size;
		}
	}

	printf("result: %li\n", max_size);
	return (0);
}
