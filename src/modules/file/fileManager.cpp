#include "../../include/file/fileManager.h"
#include <iostream>
#include <cstring>

FileManager::FileManager(int disk_size, vector<File *> files)
{
  this->disk_size = disk_size;
  this->remaining_size = this->disk_size;

  for (int i = 0; i < this->disk_size; i++)
    this->disk_blocks[i] = EMPTY_BLOCK;

  for (auto file : files) {
    for (int i = 0; i < file->size; i++)
      this->disk_blocks[file->start_block + i] = file->filename;
    this->remaining_size -= file->size;
  }
}
  
FileActionCode FileManager::addFile(File *file)
{
  if (file->size > this->remaining_size) return NOT_ENOUGH_SPACE;

  auto startPos = getContiguousIndex(file->size);
  if (startPos == -1) {
    compactDisk();
    startPos = getContiguousIndex(file->size);
  }

  // as we verified that there is enough space, we can just write the file
  for (int i = 0; i < file->size; i++)
    disk_blocks[startPos + i] = file->filename;

  this->remaining_size -= file->size;

  return OK;
}

FileActionCode FileManager::removeFile(char filename)
{
  auto file = getFile(filename);
  if (file == nullptr) return FILE_NOT_FOUND;

  for (int i = 0; i < file->size; i++)
  disk_blocks[file->start_block + i] = EMPTY_BLOCK;
  this->remaining_size += file->size;

  return OK;
}

File *FileManager::getFile(char filename)
{
  int start_block = -1, size = 0;
  for (int i = 0; i < this->disk_size; i++) {
    if (this->disk_blocks[i] == filename) {
      if (start_block == -1) start_block = i;
      size++;
    } else if (start_block != -1) break;
  }

  if (start_block == -1) return nullptr;
  return new File(filename, start_block, size);
}

void FileManager::printDisk()
{
  string printStr = to_string(SEPARATOR);
  for (int i = 0; i < this->disk_size; i++)
    printStr += this->disk_blocks[i] + SEPARATOR;
  print(printStr);
}

int FileManager::getContiguousIndex(int size) {
  int emptyCount = 0, startPos = -1;

  for (int i = 0; i < this->disk_size; ++i) {
    if (this->disk_blocks[i] == EMPTY_BLOCK) {
      if (startPos == -1) {
        startPos = i;
      }
      emptyCount++;
    } else {
      if (emptyCount >= size) break;
      emptyCount = 0, startPos = -1;
    }
  }

  if (emptyCount >= size) return startPos;
  return -1;
}

void FileManager::compactDisk() {
  int writeIndex = 0;

  for (int i = 0; i < this->disk_size; i++) {
    if (this->disk_blocks[i] != EMPTY_BLOCK)
      this->disk_blocks[writeIndex++] = this->disk_blocks[i];
  }

  for (; writeIndex < this->disk_size; writeIndex++) {
    this->disk_blocks[writeIndex] = EMPTY_BLOCK;
  }
}
