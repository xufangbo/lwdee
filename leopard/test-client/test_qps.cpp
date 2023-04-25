#include "test_qps.hpp"
#include "core/log.hpp"
#include "thread"

TestReport qps_report;
TestInput qps_input("1K", 1000);
TestSync qps_sync;
TestAsync qps_async;
TestLongConnect qps_long;

void task(std::string ip, int port);

void test_qps(std::string ip, int port) {
  for (int i = 0; i < 1; i++) {
    task(ip,port);
    sleep(5);
    // for (int i = 0; i < 10; i++) {
    //   std::thread t(task, ip, port);
    //   t.detach();
    //   usleep(1000 * 100);
    // }    
  }
}

void task(std::string ip, int port) {
  qps_async(qps_report, 100, qps_input, 1, ip, port, 30);
}