#include "../include/fileManager.h"

FileManager::FileManager()
{
  printd("FileManager::FileManager()");
}

void FileManager::setup(int disk_size, int occuped_blocks, vector<File *> files, vector<Instruction *> instructions)
{
  for (auto instruction : instructions) {
    printd("FileManager::setup - instruction: " + to_string(instruction->pid) + " " + to_string(instruction->opcode) + " " + instruction->filename + " " + to_string(instruction->numBlocks));
  }
}
