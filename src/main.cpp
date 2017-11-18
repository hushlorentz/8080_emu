#include <string>

#include "cpu.h"
#include "io.h"

using namespace std;

int FILE_SIZE = 8192;
string INPUT_FILE = "data/invaders.bin";

int main(void) 
{
  unsigned char buffer[FILE_SIZE];
  memset(buffer, 0, FILE_SIZE);
  CPU cpu;

  if (openFile(INPUT_FILE, buffer, FILE_SIZE)  == 0) 
  {
    cpu.loadProgram(buffer, FILE_SIZE);
    cpu.processProgram();
  }

  return 0;
}
