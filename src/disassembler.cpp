#include <iomanip>
#include <iostream>

#include "disassembler.h"

using namespace std;

void Disassembler::translateOpCodes(unsigned char *buffer, int bufferSize) 
{
  for (unsigned char *p = buffer; p < buffer + bufferSize; p++) 
  {
    cout << hex << setfill('0') << setw(2) << (int)*p << ": ";

    switch (*p)
    {
      case 0x0:
        cout << "NOP";
        break;
      case 0x37:
        cout << "STC - Set Carry";
        break;
      case 0x3f:
        cout << "CMC - Compliment Carry";
        break;
      default:
        cout << "Unsupported OpCode";
        break;
    }

    cout << endl;
  }
}
