#ifndef UNHANDLED_OP_CODE_EXCEPTION_H
#define UNHANDLED_OP_CODE_EXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class UnhandledOpCodeException: public exception
{
  public:
    UnhandledOpCodeException(uint8_t opCode);
    virtual const char *what() const throw();
  private:
    uint8_t opCode;
};

#endif
