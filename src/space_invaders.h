#ifndef SPACE_INVADERS_H
#define SPACE_INVADERS_H

#include "cpu.h"
#include "port_handler.h"

#define BUTTON_COIN 1
#define BUTTON_START 4
#define BUTTON_SHOOT 16
#define BUTTON_LEFT 32
#define BUTTON_RIGHT 64

class SpaceInvaders : public PortHandler
{
  public:
    SpaceInvaders();
    virtual uint8_t outputPortHandler(uint8_t address, uint8_t value);
    virtual uint8_t inputPortHandler(uint8_t address);
    uint16_t registerX;
    uint8_t shiftOffset;
    uint8_t inputRegister;
    void buttonPressed(uint8_t button);
    void buttonReleased(uint8_t button);
};

#endif
