#include "../include/utils.h"

void Utils::sleep(int time)
{
  print("Utils::sleep()");
}

double Utils::randomDouble(double min, double max)
{
  random_device rd;  // Create a random device
	mt19937 gen(rd());  // Create a Mersenne Twister pseudo-random number generator
	uniform_real_distribution<> dis(min, max);

	return dis(gen);
}
