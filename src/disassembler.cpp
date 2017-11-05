#include <iomanip>
#include <iostream>

#include "disassembler.h"
#include "op_codes.h"

using namespace std;

void Disassembler::translateOpCodes(unsigned char *buffer, int bufferSize) 
{
  for (unsigned char *p = buffer; p < buffer + bufferSize; p++) 
  {
    cout << hex << setfill('0') << setw(2) << (int)*p << ": ";

    switch (*p)
    {
      case NOP:
        cout << "NOP";
        break;
      case STC:
        cout << "STC - Set Carry";
        break;
      case CMC:
        cout << "CMC - Compliment Carry";
        break;
      default:
        cout << "Unsupported OpCode";
        break;
    }

    cout << endl;
  }
}
