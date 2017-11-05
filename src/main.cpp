#include <string>

#include "disassembler.h"
#include "io.h"

using namespace std;

int FILE_SIZE = 2048;
string INPUT_FILE = "data/invaders.e";

int main(void) 
{
  unsigned char buffer[FILE_SIZE];
  memset(buffer, 0, FILE_SIZE);

  if (openFile(INPUT_FILE, buffer, FILE_SIZE)  == 0) 
  {
    Disassembler::translateOpCodes(buffer, FILE_SIZE);
  }

  return 0;
}
