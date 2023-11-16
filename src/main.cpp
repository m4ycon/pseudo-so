#include "./include/common.h"
#include "./include/memoryManager.h"
#include "./include/scheduler.h"
#include "./include/resourceManager.h"
#include "./include/fileManager.h"
#include "./include/process.h"


bool DEBUG = false;

int main(int argc, char const *argv[]) {
  std::ios::sync_with_stdio(true);
  cin.tie(NULL);

  if (argc > 1) {
    if (string(argv[1]) == "-d") {
      DEBUG = true;
    }
    // espaço para mais flags se necessário
  }

  // auto memoryManager = new MemoryManager();
  // auto scheduler = new Scheduler();
  // auto resourceManager = new ResourceManager();
  // auto fileManager = new FileManager();
  
  auto process1 = new Process(1, 0, 0, 0);
  auto process2 = new Process(2, 0, 0, 0);
  auto process3 = new Process(3, 0, 0, 0);

  thread t1 = thread(&Process::run, process1);
  thread t2 = thread(&Process::run, process2);
  thread t3 = thread(&Process::run, process3);

  t1.join();
  t2.join();
  t3.join();

  return 0;
}
