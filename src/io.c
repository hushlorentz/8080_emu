#include <stdio.h>

int openFile(const char *filePath, unsigned char *buffer, int bufferSize)
{
  FILE *input = fopen(filePath, "r");  

  if (input) {
    int readCount = fread(buffer, bufferSize, 1, input);
    fclose(input);

    if (readCount != 1) {
      printf("Number of bytes read does not match number expected!\n");
      return 1;
    }
  } else {
    return 1;
  }

  return 0;
}
