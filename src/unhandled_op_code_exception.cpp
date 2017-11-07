#include <iomanip>
#include <sstream>

#include "unhandled_op_code_exception.h"

UnhandledOpCodeException::UnhandledOpCodeException(uint8_t opCode) : opCode(opCode)
{
}

const char *UnhandledOpCodeException::what() const throw()
{
  stringstream stream;
  string message = "Unhandled Op Code: 0x";

  stream << hex << setfill('0') << setw(2) << (int)opCode;
  return (message + stream.str()).c_str();
}
