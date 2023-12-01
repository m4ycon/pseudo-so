#pragma once
#include "../common.h"
#include "./file.h"
#include "../process/instruction.h"

enum FileActionCode
{
  OK,
  FILE_NOT_FOUND,
  NOT_ENOUGH_SPACE,
  FILE_ALREADY_EXISTS
};
class FileManager
{
public:
  FileManager(int disk_size, vector<File *> files);

  FileActionCode addFile(File *file);
  FileActionCode removeFile(char filename);
  File* getFile(char filename);

  void printDisk();

private:
  int disk_size, remaining_size;
  char disk_blocks[DEF_MEMORY_SIZE];

  void compactDisk();
  int getContiguousIndex(int size);
};
