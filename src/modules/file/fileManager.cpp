#include "../../include/file/fileManager.h"

FileManager::FileManager(int disk_size, vector<File *> files)
{
  printd("FileManager::FileManager()");

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
  
  // TODO: verificar se tem um espaço contíguo suficiente, se não tiver, usar o compactDisk()
  // TODO: grava o arquivo no primeiro espaço contíguo que encontrar

  for (int i = 0; i < file->size; i++)
    disk_blocks[file->start_block + i] = file->filename;

  return OK;
}

FileActionCode FileManager::removeFile(string filename)
{
  auto file = getFile(filename);
  if (file == NULL) return FILE_NOT_FOUND;

  for (int i = 0; i < file->size; i++)
    disk_blocks[file->start_block + i] = EMPTY_BLOCK;
  this->remaining_size += file->size;

  return OK;
}

File *FileManager::getFile(string filename)
{
  int start_block = -1, size = 0;
  for (int i = 0; i < this->disk_size; i++) {
    if (this->disk_blocks[i] == filename) {
      if (start_block == -1) start_block = i;
      size++;
    } else if (start_block != -1) break;
  }

  if (start_block == -1) return NULL;
  return new File(filename, start_block, size);
}

void FileManager::printDisk()
{
  printd("FileManager::printDisk()");

  string printStr = "|";
  for (int i = 0; i < this->disk_size; i++)
    printStr += this->disk_blocks[i] + '|';
  print(printStr);
}

void FileManager::compactDisk()
{
}
