#include "../include/instruction.h"

Instruction::Instruction(int pid, int opcode, string filename, int numBlocks)
{
  this->pid = pid;
  this->opcode = opcode;
  this->filename = filename;
  this->numBlocks = numBlocks;
}
