#include "stdio.h"
#include "stdlib.h"

#define CONNECTIONS 499500

typedef struct s_pos
{
	int x;
	int y;
	int z;
	int circuit;
} t_pos;

typedef struct s_dist
{
	long  dist;
	t_pos *pos1;
	t_pos *pos2;
} t_dist;

int comp(const void *a, const void *b)
{
	t_dist *aa = (t_dist *) a;
	t_dist *bb = (t_dist *) b;

	return (aa->dist > bb->dist);
}

int comp_int(const void *a, const void *b)
{
	return (*(int *) a < *(int *) b);
}

int compare_pos(t_pos pos1, t_pos pos2)
{
	if ((pos1.x == pos2.x) && (pos1.y == pos2.y) && (pos1.z == pos2.z))
		return (1);
	return (0);
}

int main()
{
	FILE *file;

	file = fopen("input", "r");

	t_pos pos[1001];

	int	 n = 0;
	char buffer[30];
	while (fgets(buffer, sizeof(buffer), file) != NULL)
	{
		sscanf(buffer, "%i,%i,%i\n", &pos[n].x, &pos[n].y, &pos[n].z);
		pos[n].circuit = -1;
		// printf("%s", buffer);
		n++;
	}

	// formula for unique pairs for given n: n(n -1)/2
	int		size = ((n * (n - 1)) / 2);
	t_dist *dist = (t_dist *) malloc(sizeof(t_dist) * size);
	// t_dist dist[190];

	int nn = 0;
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			// not squaresd since it won't change comparison result
			long xx = pos[i].x - pos[j].x;
			long yy = pos[i].y - pos[j].y;
			long zz = pos[i].z - pos[j].z;
			dist[nn].dist = xx * xx + yy * yy + zz * zz;
			dist[nn].pos1 = &pos[i];
			dist[nn].pos2 = &pos[j];
			nn++;
		}
	}
	qsort(dist, size, sizeof(t_dist), comp);

	int new_circuit = 2;
	// dist[0].circuit = 1;
	dist[0].pos1->circuit = 1;
	dist[0].pos2->circuit = 1;
	int ii = 1;
	int circuit_found = 0;
	while (ii < CONNECTIONS)
	{
		if (dist[ii].pos1->circuit == -1 && dist[ii].pos2->circuit == -1)
		{
			dist[ii].pos1->circuit = new_circuit;
			dist[ii].pos2->circuit = new_circuit;
			new_circuit++;
		}
		else
		{
			if (dist[ii].pos1->circuit == dist[ii].pos2->circuit)
			{
				ii++;
				continue;
			}
			else if (dist[ii].pos1->circuit == -1)
			{
				dist[ii].pos1->circuit = dist[ii].pos2->circuit;
			}
			else if (dist[ii].pos2->circuit == -1)
			{
				dist[ii].pos2->circuit = dist[ii].pos1->circuit;
			}
			else if (dist[ii].pos1->circuit > dist[ii].pos2->circuit)
			{
				int change = dist[ii].pos1->circuit;
				for (int i = 0; i < n; i++)
				{
					if (pos[i].circuit == change)
						pos[i].circuit = dist[ii].pos2->circuit;
				}
			}
			else if (dist[ii].pos2->circuit > dist[ii].pos1->circuit)
			{
				int change = dist[ii].pos2->circuit;
				for (int i = 0; i < n; i++)
				{
					if (pos[i].circuit == change)
						pos[i].circuit = dist[ii].pos1->circuit;
				}
			}
			new_circuit = 0;
			for (int i = 0; i < n; i++)
			{
				if (pos[i].circuit > new_circuit)
					new_circuit = pos[i].circuit;
			}
			new_circuit++;
		}
		int part2 = 1;
		for (int i = 0; i < n; i++)
		{
			if (pos[i].circuit != 1)
			{
				part2 = 0;
				break;
			}
		}
		if (part2)
		{
			printf("part2: %li\n", (long)dist[ii].pos1->x * (long)dist[ii].pos2->x);
		}
		ii++;
	}

	int result = 1;
	int sizes[300];
	for (int circuit = 1; circuit < new_circuit; circuit++)
	{
		int temp = 0;
		for (int i = 0; i < n; i++)
		{
			if (pos[i].circuit == circuit)
				temp++;
		}
		sizes[circuit] = temp;
	}
	qsort(sizes, new_circuit, sizeof(int), comp_int);
	printf("result\t: %i\n", sizes[0] * sizes[1] * sizes[2]);
	return (0);
}
