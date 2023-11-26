#include "./include/common.h"
#include "./include/memoryManager.h"
#include "./include/scheduler.h"
#include "./include/resourceManager.h"
#include "./include/fileManager.h"
#include "./include/process.h"
#include "./include/fileReader.h"

extern bool DEBUG;

int main(int argc, char const *argv[]) {
  ios::sync_with_stdio(true);
  cin.tie(NULL);

  if (argc > 1) {
    if (string(argv[1]) == "-d") {
      DEBUG = true;
    }
    // espaço para mais flags se necessário
  }

  auto scheduler = new Scheduler();
  auto fileManager = new FileManager();

  auto fileReader = new FileReader();
  fileReader->setup("./input/processes.txt", "./input/files.txt", scheduler, fileManager);

  return 0;


  // auto memoryManager = new MemoryManager();
  // auto resourceManager = new ResourceManager();

  // for (int i = 0; i < 10000; i++) {
  //   int priority = Utils::randomDouble(5, 1000);
  //   auto process = new Process(0, priority, 0, 0, false, false, false, 0);
  //   scheduler->addProcess(process);
  // }

  // print("realtimeQueue: " + to_string(scheduler->realtimeQueue.size()));
  // print("userQueue1: " + to_string(scheduler->userQueue1.size()));
  // print("userQueue2: " + to_string(scheduler->userQueue2.size()));
  // print("userQueue3: " + to_string(scheduler->userQueue3.size()));

  // auto p = scheduler->removeProcess();
  // print("PID: " + to_string(p->getPID()) + "; priority: " + to_string(p->getProcessPriority()));
  // p = scheduler->removeProcess();
  // print("PID: " + to_string(p->getPID()) + "; priority: " + to_string(p->getProcessPriority()));

  // print("realtimeQueue: " + to_string(scheduler->realtimeQueue.size()));
  // print("userQueue1: " + to_string(scheduler->userQueue1.size()));
  // print("userQueue2: " + to_string(scheduler->userQueue2.size()));
  // print("userQueue3: " + to_string(scheduler->userQueue3.size()));


  // thread t1 = thread(&Process::run, process1);
  // thread t2 = thread(&Process::run, process2);
  // thread t3 = thread(&Process::run, process3);

  // t1.join();
  // t2.join();
  // t3.join();

  return 0;
}
