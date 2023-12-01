#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore>
#include <random>
#include <string>
#include <queue>
#include <map>

#include "./utils/utils.h"

using namespace std;

#define MAX_QUEUE_SIZE 1000
#define DEF_MEMORY_SIZE 10000
#define QUANTUM 1 // ms

const char EMPTY_BLOCK = ' ';
const char SEPARATOR = '|';
