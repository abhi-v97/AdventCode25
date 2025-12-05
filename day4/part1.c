#include "stdio.h"
#include "fcntl.h"
#include "unistd.h"
#include "stdlib.h"

// create a temporary map copy to iterate over
char **map_input(char *input)
{
  char **map = (char **) malloc(sizeof(char *) * 141);
  map[140] = NULL;

  int fd = open(input, O_RDONLY);

  for (int i = 0; i < 140; i++)
  {
    char *buffer = (char *) malloc(sizeof(char) * 141);
    ssize_t bytes = read(fd, buffer, 141);
    buffer[140] = '\0';
    
    if (bytes < 0)
      break;

    map[i] = buffer;
    read (fd, NULL, 1);
  }
  return (map);
}

// returns true if the following conditions aren't true:
// - i and j equal zero: would cause the square to check itself
// - xx or yy are out of map bounds, below zero or above maximum
int check_bounds(int x, int y, int i, int j)
{
  int xx = x + i;
  int yy = y + j;
  
  if (i == 0 && j == 0)
	return (1);
  if (xx < 0 || xx > 139)
    return (1);
  if (yy < 0 || yy > 139)
    return (1);
  return (0);
}

// checks each surrounding squares for the '@' character
// the outer for loop i refers to the vertical axis
// the inner for loop j refers to the horizontal axis
int calc_neighbours(char **map, int x, int y)
{
  int count = 0;

  for (int i = -1; i < 2; i++)
  {
    for (int j = -1; j < 2; j++)
    {
      if (check_bounds(x, y, i, j))
        continue;
      if (map[x + i][y + j] == '@')
        count++;
    }
  }
  if (count < 4)
    return (1);
  return (0);
}

int main()
{
  char **map = map_input("input");

  int result = 0;
  for (int x = 0; x < 140; x++)
  {
    for (int y = 0; y < 140; y++)
      {
        if (map[x][y] == '@')
          result += calc_neighbours(map, x, y);
      }
  }
  printf("result: %i\n", result);
  return (0);
}
