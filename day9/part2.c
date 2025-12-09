#include "limits.h"
#include "stdio.h"
#include "stdlib.h"

#define SIZE 496

typedef struct s_pos
{
	int x;
	int y;
} t_pos;

int comp_x(const void *a, const void *b)
{
	const t_pos *const *aa = (const t_pos *const *) a;
	const t_pos *const *bb = (const t_pos *const *) b;

	if ((*aa)->x < (*bb)->x)
		return -1;
	if ((*aa)->x > (*bb)->x)
		return 1;
	return 0;
}

int comp_y(const void *a, const void *b)
{
	const t_pos *const *aa = (const t_pos *const *) a;
	const t_pos *const *bb = (const t_pos *const *) b;

	if ((*aa)->y < (*bb)->y)
		return -1;
	if ((*aa)->y > (*bb)->y)
		return 1;
	return (0);
}

int check_valid(int k, int i, int j)
{
	int a = i, b = j;
	if (b > a)
	{
		int t = a;
		a = b;
		b = t;
	}
	if (k > a && k < b)
		return 1;
	return 0;
}

static int point_on_segment(const t_pos *a, const t_pos *b, int px, int py)
{
	long long ax = a->x, ay = a->y;
	long long bx = b->x, by = b->y;

	// cross product: a x b = 0 if a b are parallel
	if ((px - ax) * (by - ay) != (py - ay) * (bx - ax))
		return (0);

	// now check if its on the same line segment
	if ((px < ax && px < bx) || (px > ax && px > bx) || (py < ay && py < by) ||
		(py > ay && py > by))
		return (0);

	return (1);
}

// https://observablehq.com/@tmcw/understanding-point-in-polygon
int point_in_polygon(const t_pos *pos, int n, int px, int py)
{
	int inside;

	inside = 0;

	// is pos a red light (existing vertex)
	for (int i = 0; i < n; i++)
		if (pos[i].x == px && pos[i].y == py)
			return (1);

	// is pos a green light (inbetween red lights)
	for (int i = 0, j = n - 1; i < n; j = i++)
		if (point_on_segment(&pos[j], &pos[i], px, py))
			return (1);

	// now do the full path tracing
	for (int i = 0, j = n - 1; i < n; j = i++)
	{
		int xi = pos[i].x;
		int yi = pos[i].y;
		int xj = pos[j].x;
		int yj = pos[j].y;

		int yi_gt = yi > py;
		int yj_gt = yj > py;
		if (yi_gt != yj_gt)
		{
			long dx = (xj - xi);
			long dy = (yj - yi);

			long intersect_x = xi + dx * (py - yi) / dy;
			if (px < intersect_x)
				inside = !inside;
		}
	}
	return (inside);
}

int check_rect(int ix, int iy, int jx, int jy, t_pos pos[SIZE], int n)
{
	if (ix > jx)
	{
		int swap = ix;
		ix = jx;
		jx = swap;
	}
	if (iy > jy)
	{
		int swap = iy;
		iy = jy;
		jy = swap;
	}

	// check each vertex of rectangle first
	// no need to check each and every point in rectangle if one of the
	// vertices are sticking out
	if (!point_in_polygon(pos, n, jx, jy))
		return (1);
	if (!point_in_polygon(pos, n, jx, iy))
		return (1);
	if (!point_in_polygon(pos, n, ix, jy))
		return (1);
	if (!point_in_polygon(pos, n, ix, iy))
		return (1);

	ix++, iy++;
	jx--, jy--;
	// now check if any edge lies outside the polygon
	for (int y = iy; y < jy; y++)
	{
		if (!point_in_polygon(pos, n, ix, y))
			return (1);
	}
	for (int y = iy; y < jy; y++)
	{
		if (!point_in_polygon(pos, n, jx, y))
			return (1);
	}
	for (int x = ix; x < jx; x++)
	{
		if (!point_in_polygon(pos, n, x, iy))
			return (1);
	}
	for (int x = ix; x < jx; x++)
	{
		if (!point_in_polygon(pos, n, x, jy))
			return (1);
	}
	return 0;
}

int main()
{
	FILE *file;

	file = fopen("input", "r");
	char buffer[30];

	t_pos pos[SIZE];
	// t_pos *x_sorted[SIZE];
	// t_pos *y_sorted[SIZE];

	int n = 0;
	while (fgets(buffer, sizeof(buffer), file) != NULL)
	{
		sscanf(buffer, "%i,%i\n", &pos[n].x, &pos[n].y);
		// x_sorted[n] = &pos[n];
		// y_sorted[n] = &pos[n];
		n++;
	}

	n--;
	// qsort(x_sorted, n, sizeof(t_pos *), comp_x);
	// qsort(y_sorted, n, sizeof(t_pos *), comp_y);

	// int is_inside = point_in_polygon(pos, n, 7, 5);
	// printf("point (9,4) inside %i\n", is_inside);

	// for (int i = 0; i < 9; i++)
	// {
	// 	for (int j = 0; j < 14; j++)
	// 	{
	// 		if (point_in_polygon(pos, n, j, i))
	// 			printf("x");
	// 		else
	// 		 	printf(".");
	// 	}
	// 	printf("\n");
	// }

	// return (0);

	t_pos	  pos1;
	t_pos	  pos2;
	long long max_size = 0;
	int		  invalid;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (i == j)
				continue;
			invalid = 0;
			for (int k = 0; k < n; k++)
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
			long	  xx = abs(pos[i].x - pos[j].x) + 1;
			long	  yy = abs(pos[i].y - pos[j].y) + 1;
			long long temp_size = xx * yy;
			if (invalid || temp_size <= max_size)
				continue;
			if (!check_rect(pos[i].x, pos[i].y, pos[j].x, pos[j].y, pos, n))
				max_size = temp_size;
			printf("%i, %i\n", i, j);
		}
		printf("%i, %lli\n", i, max_size);
	}

	printf("result: %lli\n", max_size);
	return (0);
}
