#pragma once
#include "../common.h"

class FileInstruction {
  public:
    FileInstruction(int pid, int opcode, string filename, int numBlocks = 0);
    
    int pid;
    int opcode;
    string filename;
    int numBlocks;

  private:
};
