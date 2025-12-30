#include <stdio.h>

int main()
{
	FILE *file = fopen("input", "r");

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
			dial = (dial + n) % 100;
		}
		else if (dir == 'L')
		{
			dial = (dial - n) % 100;
		}
		if (dial == 0)
			count++;
		// printf("dir = %c, n = %i\n", dir, n);
	}
	printf("result = %i\n", count);
	return (0);
}
