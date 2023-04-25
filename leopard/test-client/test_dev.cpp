#include "test_dev.hpp"

#include "core/log.hpp"

TestReport dev_report;
TestInput dev_input("1K", 1000);
TestSync dev_sync;
TestAsync dev_async;

void task(std::string ip, int port);

void test_dev(std::string ip, int port) {
  for (int i = 0; i < 10; i++) {
    dev_async(dev_report, 300, dev_input, 2, ip, port, 100);
  }
}