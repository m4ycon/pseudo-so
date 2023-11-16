#pragma once
#include <random>
#include <mutex>
#include <iostream>

class Utils
{
public:
  static void sleep(double time);
  static double randomDouble(double min, double max);
};

void print(const std::string &message);
void printd(const std::string &message);
