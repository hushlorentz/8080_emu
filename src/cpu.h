#ifndef CPU_H
#define CPU_H

class CPU
{
  public:
    CPU();
    char carryBit();
    bool allClear();
    void processProgram(unsigned char *program, int programSize);

  private:
    char status;
    void setStatus(char bit);
    void flipStatusBit(char bit);
};

#endif
