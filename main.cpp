#include "./common/index.hpp"

bool DEBUG = false;

int main(int argc, char const *argv[]) {
  if (argc > 1) {
    if (string(argv[1]) == "-d") {
      DEBUG = true;
    }
    // espaço para mais flags se necessário
  }

  return 0;
}
