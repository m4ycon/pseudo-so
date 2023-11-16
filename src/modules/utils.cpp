#include "../include/utils.h"

void Utils::sleep(double seconds)
{
  printd("Utils::sleep()");
  Sleep(seconds * 1e3); // milliseconds
}

double Utils::randomDouble(double min, double max)
{
  std::random_device rd;  // Create a random device
  std::mt19937 gen(rd()); // Create a Mersenne Twister pseudo-random number generator
  std::uniform_real_distribution<> dis(min, max);

  return dis(gen);
}
