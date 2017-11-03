#include <stdio.h>

int processOpCode(unsigned char *opCodes, int programCounter) {
  return 1;
}

void disassemble(unsigned char *buffer, int bufferSize) {
  printf("We disassemble!\n");

  for (unsigned char *p = buffer; p < buffer + bufferSize; p++) {
    printf("%02x ", *p);
  }

  printf("\n");
}
