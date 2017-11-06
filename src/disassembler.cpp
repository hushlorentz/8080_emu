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
      case INR_B:
        cout << "INR_B - Increment register B";
        break;  
      case INR_C:
        cout << "INR_C - Increment register C";
        break;  
      case INR_D:
        cout << "INR_D - Increment register D";
        break;  
      case INR_E:
        cout << "INR_E - Increment register E";
        break;  
      case INR_H:
        cout << "INR_H - Increment register H";
        break;  
      case INR_L:
        cout << "INR_L - Increment register L";
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
