#include <exception>

using namespace std;

class UnhandledOpCodeException: public exception
{
  public:
    virtual const char *what() const throw();
};
