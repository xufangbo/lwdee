#include <chrono>
#include <iostream>
using namespace std;

int main() {
  chrono::system_clock::time_point now = chrono::system_clock::now();
  time_t timestamp = chrono::system_clock::to_time_t(now);
  cout << timestamp << endl;

  time_t t = time(NULL);
  //   t += 8 * 60 * 60;

  cout << t << endl;

  time_t time1 = time(0);
  cout << "time1 = " << time1 << endl;  // 1498122787
  char* strTime = ctime(&time1);
  cout << "strTime = " << strTime << endl;  // Thu Jun 22 17:13:07 2017

  time_t startTime = 1498122787;
  double betweenSecond = difftime(time1, startTime);    // 该函数返回 time1 和 time2 之间相差的秒数。
  cout << "betweenSecond = " << betweenSecond << endl;  // Thu Jun 22 17:13:07 2017

  return 0;
}