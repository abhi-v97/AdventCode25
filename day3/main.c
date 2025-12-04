#include "stdio.h"
#include "stdlib.h"
#include "math.h"

long long getLargest(char *buffer, int digits)
{
  long long result = 0;
  int pos = 0;
  
  digits--;
  while (digits >= 0)
  {
    int temp = 1;

    for (int i = pos; buffer[i] && i < (101 - digits - 1); i++) 
    {
      if ((buffer[i] - '0') > temp)
      {
        temp = (buffer[i] - '0');
        pos = i;
      }
    }
    result += temp * pow(10, digits);
    digits--;
    pos++;
  }
  return (result);
}

void printResult(int digits)
{
  FILE *file;
  size_t result;

  file = fopen("input", "r");
  if (file < 0)
    return ;

  size_t size = 100;
  char *buffer;

  buffer = (char *) malloc(sizeof(char) * 101);
  buffer[100] = '\0';

  if (buffer == NULL)
    return ;

  int pos = 0;

  int bytes = getline(&buffer, &size, file);
  result = 0;

  while (bytes > 0)
  {
    pos = 0;

    result += getLargest(buffer, digits);
    bytes = getline(&buffer, &size, file);
    buffer[100] = '\0';
  }
  printf("Digits: %i, Total voltage: %zu\n", digits, result);
}

int main()
{
  printResult(2);
  printResult(12);
  return (0);
}

