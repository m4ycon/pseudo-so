#include "../../include/file/fileManager.h"

FileManager::FileManager(int disk_size, vector<File *> files, vector<FileInstruction *> instructions)
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

  printDisk();

  for (auto instruction : instructions) {
    if (instruction->opcode == 0) {
      // create
      auto resCode = addFile(new File(instruction->filename, 0, instruction->numBlocks));
      
      if (resCode == OK) {
        auto file = getFile(instruction->filename);
        
        auto printStr = "O processo " + to_string(instruction->pid) + " criou o arquivo " + file->filename + " (blocos ";
        for (int i = 0; i < file->size; i++)
          printStr += to_string(file->start_block + i) + (i + 1 != file->size ? ", " : ").");

        print(printStr);
        continue;
      }

      if (resCode == NOT_ENOUGH_SPACE) {
        print("O processo " + to_string(instruction->pid) + " não pode criar o arquivo " + instruction->filename + " (falta de espaço).");
        continue;
      }
    } else {
      // delete
      auto resCode = removeFile(instruction->filename);

      if (resCode == OK) {
        print("O processo " + to_string(instruction->pid) + " deletou o arquivo " + instruction->filename + ".");
        continue;
      }

      if (resCode == FILE_NOT_FOUND) {
        print("O processo " + to_string(instruction->pid) + " não pode deletar o arquivo " + instruction->filename + " porque ele não existe.");
        continue;
      }
    }
  }

  printDisk();
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
  int start_block = 0, size = 0;
  for (int i = 0; i < this->disk_size; i++) {
    if (this->disk_blocks[i] == filename) {
      if (start_block == 0) start_block = i;
      size++;
    }
  }

  if (start_block == 0) return NULL;
  return new File(filename, start_block, size);
}

void FileManager::printDisk()
{
  printd("FileManager::printDisk()");

  print("|", false);
  for (int i = 0; i < this->disk_size; i++) {
    print(this->disk_blocks[i] + '|', false);
  }
  print();
}

void FileManager::compactDisk()
{
}
