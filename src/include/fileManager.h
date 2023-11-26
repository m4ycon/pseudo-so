#pragma once
#include "./common.h"
#include "./file.h"
#include "./instruction.h"

class FileManager {
  public:
    FileManager();

    void setup(int disk_size, int occuped_blocks, vector<File *> files, vector<Instruction *> instructions);
  private:
};
