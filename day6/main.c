#include <stddef.h>
#include <string.h>

#include "stdio.h"
#include "stdlib.h"

int num_digits(int n)
{
	int digits = 0;

	if (n == 0)
		return (1);
	if (n < 0)
	{
		n = -n;
		digits++;
	}
	while (n > 0)
	{
		digits++;
		n /= 10;
	}
	return (digits);
}

long ceph_multiply(long map[5][2000], int pos)
{
	long result = 1;
	int	 max_digit = 0;
	long temp = 0;

	for (int i = 0; i < 4; i++)
	{
		if (num_digits(map[i][pos]) > max_digit)
			max_digit = num_digits(map[i][pos]);
	}

	while (max_digit > 0)
	{
		temp = 0;
		for (int i = 0; i < 4; i++)
		{
			if (!map[i][pos])
				continue;
			temp = temp * 10 + map[i][pos] % 10;
			map[i][pos] /= 10;
		}
		result *= temp;
		max_digit--;
	}

	return (result);
}

long ceph_add(long map[5][2000], int pos)
{
	long result = 0;
	int	 max_digit = 0;
	long temp = 0;

	for (int i = 0; i < 4; i++)
	{
		if (num_digits(map[i][pos]) > max_digit)
			max_digit = num_digits(map[i][pos]);
	}

	while (max_digit > 0)
	{
		temp = 0;
		for (int i = 0; i < 4; i++)
		{
			if (!map[i][pos])
				continue;
			temp = temp * 10 + map[i][pos] % 10;
			map[i][pos] /= 10;
		}
		result += temp;
		max_digit--;
	}

	return (result);
}

int main()
{
	long  map[5][2000];
	FILE *file = fopen("input", "r");

	char   *buffer = NULL;
	size_t	bytes = 0;
	ssize_t read = 0;
	read = getline(&buffer, &bytes, file);
	int x = 0;
	int y = 0;
	while (x < 4)
	{
		y = 0;
		for (int i = 0; i < read; i++)
		{
			if (buffer[i] && buffer[i] == ' ')
				continue;
			map[x][y] = atoi(buffer + i);
			i += num_digits(map[x][y]);
			y++;
		}
		x++;
		// free(buffer);
		buffer = NULL;
		bytes = 0;
		read = getline(&buffer, &bytes, file);
	}

	y = 0;
	for (int i = 0; i < read; i++)
	{
		if (buffer[i] && buffer[i] == ' ')
			continue;
		map[x][y] = buffer[i];
		y++;
	}

	unsigned long result = 0;
	for (int i = 0; i < 1001; i++)
	{
		if (map[4][i] == '*')
		{
			result += (map[0][i] * map[1][i] * map[2][i] * map[3][i]);
		}
		else if (map[4][i] == '+')
		{
			result += (map[0][i] + map[1][i] + map[2][i] + map[3][i]);
		}
	}
	printf("result %li\n", result);

	unsigned long result2 = 0;
	for (int i = 0; i < 1001; i++)
	{
		if (map[4][i] == '*')
		{
			result2 += ceph_multiply(map, i);
		}
		else if (map[4][i] == '+')
		{
			result2 += ceph_add(map, i);
		}
	}
	printf("result %li\n", result2);
	return (0);
}
