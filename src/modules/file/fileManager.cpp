#include "../../include/file/fileManager.h"
#include <iostream>
#include <cstring>

/**
 * @brief Construct a new File Manager:: File Manager object
 * 
 * @param disk_size 
 * @param files 
 */
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

/**
 * @brief Salva um arquivo no disco e retorna OK se salvou corretamente,
 *        NOT_ENOUGH_SPACE caso o arquivo for maior que o espaço
 *        disponível ou FILE_ALREADY_EXISTS caso o arquivo tenha o mesmo
 *        nome que um arquivo já existente.
 * 
 * @param file 
 * @return FileActionCode 
 */
FileActionCode FileManager::addFile(File *file)
{
  if (file->size > this->remaining_size) return NOT_ENOUGH_SPACE;
  if (getFile(file->filename) != nullptr) return FILE_ALREADY_EXISTS;

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

/**
 * @brief Remove um arquivo já existente. Retorna OK caso a ação ocorreu
 *        corretamente ou FILE_NOT_FOUND caso o arquivo não exista
 * 
 * @param filename 
 * @return FileActionCode 
 */
FileActionCode FileManager::removeFile(char filename)
{
  auto file = getFile(filename);
  if (file == nullptr) return FILE_NOT_FOUND;

  for (int i = 0; i < file->size; i++)
  disk_blocks[file->start_block + i] = EMPTY_BLOCK;
  this->remaining_size += file->size;

  return OK;
}

/**
 * @brief Procura por um arquivo no disco. Se encontrou, retorna um
 *        ponteiro para ele; se não, retorna um ponteiro nulo
 * 
 * @param filename 
 * @return File* 
 */
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

/**
 * @brief Printa na tela o estado do disco no momento
 */
void FileManager::printDisk()
{
  string separator(1, SEPARATOR);
  string printStr = separator;
  for (int i = 0; i < this->disk_size; i++)
    printStr += this->disk_blocks[i] + separator;
  print(printStr);
}

/**
 * @brief retorna o primeiro indice de uma sequência vazia do disco
 *        que seja maior ou igual ao tamanho passado como argumento.
 *        Caso não encontre, retorna -1
 * 
 * @param size 
 * @return int 
 */
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

/**
 * @brief Compacta o disco deixando os blocos vazios a direita (se existirem)
 */
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
