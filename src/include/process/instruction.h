#pragma once
#include "../common.h"

class Instruction {
  public:
    Instruction(int pid, int opcode, char filename, int numBlocks = 0);
    
    int pid;
    int opcode;
    char filename;
    int numBlocks;

  private:
};
