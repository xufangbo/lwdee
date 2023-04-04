#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <chrono>
#include <iostream>

using namespace std;

int main() {
  auto ms = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
  cout << ms.count() << endl;
//   usleep(1000000);
//   cout << ms.count() << endl;

  time_t t;
  time(&t);
  cout << t << endl;

  return 0;
}
