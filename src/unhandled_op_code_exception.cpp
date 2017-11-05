#include "unhandled_op_code_exception.h"

const char *UnhandledOpCodeException::what() const throw()
{
  return "Unhandled Op Code!";
}
