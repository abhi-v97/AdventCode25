#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// Explanation:
// Keep an array of numbers, which is the same size as the buffer. 
// To initialise, add 1 to start position 'S' at the same index as the string.
// Then, read through the file line by line, stopping only when you encounter
// the '^' character.
// Once you see the split, remove the number stored at the matching index in
// the long array and add it to the adjacent indexes.
// This keeps track of each new timeline, while accounting for overlaps from
// previous timelines.
int main()
{
	FILE   *file;
	long   *path_array;
	char   *buffer;
	size_t	bytes;
	ssize_t read;
	int		split;
	long	paths;

	file = fopen("input", "r");
	path_array = calloc(sizeof(long), 142);

	// init
	bytes = 0;
	buffer = NULL;
	read = getline(&buffer, &bytes, file);
	for (int i = 0; i < read; i++)
	{
		if (buffer[i] == 'S')
		{
			buffer[i] = '|';
			path_array[i] = 1;
			break;
		}
	}

	split = 0;
	paths = 1;
	while (read > 0)
	{
		for (int j = 0; j < read; j++)
		{
			if (path_array[j] > 0 && buffer[j] == '^')
			{
				if (j > 0)
					buffer[j - 1] = '|';
				buffer[j + 1] = '|';
				paths += path_array[j];
				path_array[j - 1] += path_array[j];
				path_array[j + 1] += path_array[j];
				path_array[j] = 0;
				split++;
			}
		}
		// printf("%s", buffer);
		free(buffer);
		buffer = NULL;
		read = getline(&buffer, &bytes, file);
	}
	free(buffer);
	free(path_array);
	printf("splits\t\t: %i\n", split);
	printf("timelines\t: %li\n", paths);
	return (0);
}
