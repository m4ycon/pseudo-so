#pragma once
#include "./common.h"

class Instruction {
  public:
    Instruction(int pid, int opcode, string filename, int numBlocks = 0);
    
    int pid;
    int opcode;
    string filename;
    int numBlocks;

  private:
};
