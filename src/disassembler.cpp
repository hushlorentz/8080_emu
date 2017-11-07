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
      case DCR_B:
        cout << "DCR_B - Decrement register B";
        break;  
      case INR_C:
        cout << "INR_C - Increment register C";
        break;  
      case DCR_C:
        cout << "DCR_C - Decrement register C";
        break;  
      case INR_D:
        cout << "INR_D - Increment register D";
        break;  
      case DCR_D:
        cout << "DCR_D - Decrement register D";
        break;  
      case INR_E:
        cout << "INR_E - Increment register E";
        break;  
      case DCR_E:
        cout << "DCR_E - Decrement register E";
        break;  
      case INR_H:
        cout << "INR_H - Increment register H";
        break;  
      case DCR_H:
        cout << "DCR_H - Decrement register H";
        break;  
      case DAA:
        cout << "DAA - Decimal Adjust Accumulator";
        break;  
      case INR_L:
        cout << "INR_L - Increment register L";
        break;  
      case DCR_L:
        cout << "DCR_L - Decrement register L";
        break;
      case CMA:
        cout << "CMA - Complement accumulator";
        break;
      case INR_M:
        cout << "INR_M - Increment register M";
        break;  
      case DCR_M:
        cout << "DCR_M - Decrement register M";
        break;
      case INR_A:
        cout << "INR_A - Increment register A";
        break;  
      case DCR_A:
        cout << "DCR_A - Decrement register A";
        break;
      case STC:
        cout << "STC - Set Carry";
        break;
      case CMC:
        cout << "CMC - Compliment Carry";
        break;
      case MOV_B_B:
        cout << "MOV_B_B - move register B to register B";
        break;
      case MOV_B_C:
        cout << "MOV_B_C - move register C to register B";
        break;
      case MOV_B_D:
        cout << "MOV_B_D - move register D to register B";
        break;
      case MOV_B_E:
        cout << "MOV_B_E - move register E to register B";
        break;
      case MOV_B_H:
        cout << "MOV_B_H - move register H to register B";
        break;
      case MOV_B_L:
        cout << "MOV_B_L - move register L to register B";
        break;
      case MOV_B_M:
        cout << "MOV_B_M - move register M to register B";
        break;
      case MOV_B_A:
        cout << "MOV_B_A - move register A to register B";
        break;
      case MOV_C_B:
        cout << "MOV_C_B - move register B to register C";
        break;
      case MOV_C_C:
        cout << "MOV_C_C - move register C to register C";
        break;
      case MOV_C_D:
        cout << "MOV_C_D - move register D to register C";
        break;
      case MOV_C_E:
        cout << "MOV_C_E - move register E to register C";
        break;
      case MOV_C_H:
        cout << "MOV_C_H - move register H to register C";
        break;
      case MOV_C_L:
        cout << "MOV_C_L - move register L to register C";
        break;
      case MOV_C_M:
        cout << "MOV_C_M - move register M to register C";
        break;
      case MOV_C_A:
        cout << "MOV_C_A - move register A to register C";
        break;
      case MOV_D_B:
        cout << "MOV_D_B - move register B to register D";
        break;
      case MOV_E_B:
        cout << "MOV_E_B - move register B to register E";
        break;
      case MOV_H_B:
        cout << "MOV_H_B - move register B to register H";
        break;
      case MOV_L_B:
        cout << "MOV_L_B - move register B to register L";
        break;
      case MOV_A_B:
        cout << "MOV_A_B - move register B to register A";
        break;
      case MOV_E_M:
        cout << "MOV_E_M - move register M to register E";
        break;
      case MOV_M_B:
        cout << "MOV_M_B - move register B to register M";
        break;
      case MOV_M_A:
        cout << "MOV_M_A - move register A to register M";
        break;
      case MOV_M_M:
        cout << "MOV_M_M - move register M to itself";
        break;
      default:
        cout << "Unsupported OpCode";
        break;
    }

    cout << endl;
  }
}
