#ifndef PORT_HANDLER_H
#define PORT_HANDLER_H

#include <cstdint>

class PortHandler
{
  public:
    virtual uint8_t inputPortHandler(uint8_t address) = 0;
    virtual uint8_t outputPortHandler(uint8_t address, uint8_t value) = 0;
};

#endif
