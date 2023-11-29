#include "../../include/process/instruction.h"

Instruction::Instruction(int pid, int opcode, char filename, int numBlocks)
{
  this->pid = pid;
  this->opcode = opcode;
  this->filename = filename;
  this->numBlocks = numBlocks;
}
