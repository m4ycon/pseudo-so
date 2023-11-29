#include "../../include/file/fileManager.h"
#include <iostream>
#include <cstring>

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

  /* printd("file size: " + to_string(file->size) + "; remaining size: " + to_string(this->remaining_size) + "; disk blocks: ", false );
  for (int i = 0; i < this->disk_size; i++) {
	if (this->disk_blocks[i] == " ") {
	  printd("_", false);
	} else {
	  printd(this->disk_blocks[i], false);
	}
  } */

 /*  printd("");
  printd("disk blocks depois de compactar:", false);
  compactDisk();

  for (int i = 0; i < this->disk_size; i++) {
	if (this->disk_blocks[i] == " ") {
	  printd("_", false);
	} else {
	  printd(this->disk_blocks[i], false);
	}
  }
  printd("-", false);
  printd(""); */
  
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

void FileManager::insertString(int size, const char* str) {
  int strLength = strlen(str);
  int emptyCount = 0;
  int startPos = -1;

  for (int i = 0; i < size; ++i) {
	if (this->disk_blocks[i] == " ") {
	  if (startPos == -1) {
		startPos = i;
	  }
	  emptyCount++;
	} else {
		if (emptyCount >= strLength) {
		  break;
		}
		startPos = -1;
		emptyCount = 0;
	}
  }

  if (emptyCount >= strLength) {
	
	for (int i = 0; i < strLength; ++i) {
	  this->disk_blocks[startPos + i] = str[i];
	}

  } else {
	compactDisk();
	insertString(size, str); 
  } // TODO: fazer o tratamento de erro caso não tenha espaço suficiente mesmo depois de compactar
}

void FileManager::compactDisk() {
    int writeIndex = 0;

	int actualSize = 0;
	for (int i = 0; i < this->disk_size; i++) { // por favor trocar isso por uma função de gente
		actualSize++;
	}

    for (int readIndex = 0; readIndex < actualSize; ++readIndex) {
        if (this->disk_blocks[readIndex] != " ") {
            this->disk_blocks[writeIndex++] = this->disk_blocks[readIndex];
        }
    }
    for (; writeIndex < actualSize; ++writeIndex) {
        this->disk_blocks[writeIndex] = " ";
    }
}


