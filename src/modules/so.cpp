#include "../include/so.h"

SO::SO()
{
  auto fileReader = new FileReader();

  this->scheduler = new Scheduler();
  this->memoryManager = new MemoryManager();
  this->resourceManager = new ResourceManager();
  this->fileManager = fileReader->setup("./input/processes.txt", "./input/files.txt", scheduler);
}

void SO::exec()
{

}
