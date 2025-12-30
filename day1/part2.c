#include <stdio.h>

int main()
{
	FILE *file = fopen("example", "r");

	char	buf[20];
	size_t	bytes = 0;
	ssize_t read = 0;

	int dial = 50;
	int count = 0;
	while (fgets(buf, sizeof(buf), file))
	{
		char dir = 0;
		int	 n = 0;
		sscanf(buf, "%c%i\n", &dir, &n);

		if (dir == 'R')
		{
			if (dial + n > 100)
				// for (int temp = dial + n; temp > 100; temp -= 100)
				// 	count++;
				count += (dial + n) / 100;
			dial = (dial + n) % 100;
		}
		else if (dir == 'L')
		{
			if (dial - n < 0)
				// for (int temp = dial - n; temp < 0; temp += 100)
				// 	count++;
				count = count - (dial - n) / 100;
			dial = (dial - n) % 100;
		}
		if (dial == 0)
			count++;
		// printf("dir = %c, n = %i\n", dir, n);
	}
	printf("result = %i\n", count);
	return (0);
}
