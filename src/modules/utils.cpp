#include "../include/utils.h"

#ifdef _WIN32
  #include <windows.h> // Windows
  void Utils::sleep(double seconds)
  {
    printd("Utils::sleep()");
    Sleep(seconds * 1e3); // milliseconds
  }
#else
  #include <unistd.h> // Linux
  void Utils::sleep(double seconds)
  {
    printd("Utils::sleep()");
    usleep(seconds * 1e6); // microseconds
  }
#endif

double Utils::randomDouble(double min, double max)
{
  std::random_device rd;  // Create a random device
  std::mt19937 gen(rd()); // Create a Mersenne Twister pseudo-random number generator
  std::uniform_real_distribution<> dis(min, max);

  return dis(gen);
}
