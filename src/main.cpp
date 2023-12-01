#include "./include/common.h"
#include "./include/so.h"

extern bool DEBUG;

int main(int argc, char const *argv[]) {
  ios::sync_with_stdio(true);
  cin.tie(NULL);

  string processes_path, files_path;
  if (argc == 3) {
    processes_path = string(argv[1]);
    files_path = string(argv[2]);
  } else {
    cout << "Usage: ./main processes_path files_path" << endl;
    return 1;
  }

  auto so = new SO(processes_path, files_path);
  so->exec();

  return 0;
}
