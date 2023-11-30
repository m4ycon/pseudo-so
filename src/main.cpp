#include "./include/common.h"
#include "./include/so.h"

extern bool DEBUG;

int main(int argc, char const *argv[]) {
  ios::sync_with_stdio(true);
  cin.tie(NULL);

  if (argc > 1) {
    if (string(argv[1]) == "-d") {
      DEBUG = true;
    }
    // space for more flags
  }

  auto so = new SO();
  so->exec();

  return 0;
}
