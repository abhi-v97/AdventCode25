#include "stdio.h"

int main()
{
	char *map[5];
	FILE *file = fopen("input", "r");

	char   *buffer = NULL;
	size_t	bytes = 0;
	ssize_t read = 0;
	read = getline(&buffer, &bytes, file);
	for (int i = 0; i < 5; i++)
	{
		map[i] = buffer;
		size_t bytes = 0;
		read = getline(&buffer, &bytes, file);
	}

	char last_op = '+';

	int		  i = 0;
	long	  temp = 0;
	long	  temp2 = 0;
	long long result = 0;
	while (map[4][i])
	{
		if (map[4][i] == '\n')
			break;

		temp2 = 0;
		for (int j = 0; j < 4; j++)
		{
			if (map[j][i] == ' ')
				continue;
			temp2 = temp2 * 10 + map[j][i] - '0';
		}
		if (temp2)
		{
			if (last_op == '+')
				temp += temp2;
			else
			{
				if (temp == 0)
					temp = 1;
				temp *= temp2;
			}
		}
		else
		{
			result += temp;
			last_op = map[4][i + 1];
			temp = 0;
		}
		i++;
	}
	printf("%lli\n", result);
	return (0);
}
