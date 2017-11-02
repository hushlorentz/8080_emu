#include <assert.h>
#include <stdio.h>

#include "disassembler.h"

void disassemblerReturns1For0OpCode();

void runTests()
{
  printf("Running Tests...\n");
  disassemblerReturns1For0OpCode();
  printf("...Tests Finished\n");
}

void disassemblerReturns1For0OpCode()
{
  unsigned char opCodes[] = {0};
  assert(processOpCode(opCodes, 0) == 1);
}
