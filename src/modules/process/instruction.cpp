#include "../../include/process/instruction.h"

/**
 * @brief Construct a new Instruction:: Instruction object
 * 
 * @param pid 
 * @param opcode 
 * @param filename 
 * @param numBlocks 
 */
Instruction::Instruction(int pid, int opcode, char filename, int numBlocks)
{
  this->pid = pid;
  this->opcode = opcode;
  this->filename = filename;
  this->numBlocks = numBlocks;
}
