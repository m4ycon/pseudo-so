#include "./include/common.h"
#include "./include/memoryManager.h"
#include "./include/scheduler.h"
#include "./include/resourceManager.h"
#include "./include/fileManager.h"
#include "./include/process.h"


bool DEBUG = false;

int main(int argc, char const *argv[]) {
  if (argc > 1) {
    if (string(argv[1]) == "-d") {
      DEBUG = true;
    }
    // espaço para mais flags se necessário
  }

  auto memoryManager = new MemoryManager();
  auto scheduler = new Scheduler();
  auto resourceManager = new ResourceManager();
  auto fileManager = new FileManager();
  auto process = new Process();

  return 0;
}
