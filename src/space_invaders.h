#ifndef SPACE_INVADERS_H
#define SPACE_INVADERS_H

#include "cpu.h"
#include "port_handler.h"

class SpaceInvaders : public PortHandler
{
  public:
    SpaceInvaders();
    virtual uint8_t outputPortHandler(uint8_t address, uint8_t value);
    virtual uint8_t inputPortHandler(uint8_t address);
    uint16_t registerX;
    uint8_t shiftOffset;
};

#endif
