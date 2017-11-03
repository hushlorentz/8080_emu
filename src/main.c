#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "disassembler.h"
#include "io.h"
#include "test.h"

int FILE_SIZE = 2048;
char *inputFile = "data/invaders.e";

int main(void) {
  unsigned char buffer[FILE_SIZE];
  memset(buffer, 0, FILE_SIZE);

  runTests();

  if (openFile(inputFile, buffer, FILE_SIZE)  == 0) {
    disassemble(buffer, FILE_SIZE);
  }

  return 0;
}
