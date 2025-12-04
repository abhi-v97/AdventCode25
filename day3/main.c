#include "stdio.h"
#include "stdlib.h"
#include "math.h"

/**
* Search from left to right for the largest digit. However, you don't want to
* pick 9 if its the last number. You need to leave a gap so that you can fill
* in the rest of the digits.
*
* The cut-off is simply line_size - num_of_digits.
*
* So, you find the biggest digit just before the cutoff for each subsequent digit.
*/
long long getLargest(char *buffer, int digits)
{
  long long result = 0;
  int pos = 0;
  
  digits--;
  while (digits >= 0)
  {
    int temp = 1;

    for (int i = pos; buffer[i] && i < (100 - digits); i++) 
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
  size_t size = 100;
  char *buffer;

  file = fopen("input", "r");
  if (file < 0)
    return ;

  buffer = (char *) malloc(sizeof(char) * 101);
  buffer[100] = '\0';

  if (buffer == NULL)
    return ;

  int bytes = getline(&buffer, &size, file);

  result = 0;
  while (bytes > 0)
  {
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

