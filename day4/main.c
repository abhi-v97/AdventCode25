#include "stdio.h"
#include "fcntl.h"
#include "unistd.h"
#include "stdlib.h"

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

int main()
{
  char **map = map_input("input");

  // for (int i = 0; i < 140; i++)
  //   printf("%s\n", map[i]);

 
  return (0);
}
