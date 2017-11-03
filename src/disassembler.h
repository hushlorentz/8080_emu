#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

void disassemble(unsigned char *buffer, int bufferSize);
int processOpCode(unsigned char *opCodes, int programCounter);

#endif
