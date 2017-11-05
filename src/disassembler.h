#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

class Disassembler
{
  public:
    static void translateOpCodes(unsigned char *buffer, int bufferSize);
};

#endif
