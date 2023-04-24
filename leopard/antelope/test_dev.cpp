#include "test_dev.hpp"

#include "core/log.hpp"

TestReport dev_report;
TestInput dev_input("1K", 1000);
TestSync dev_sync;

void task(std::string ip, int port);

void test_dev(std::string ip, int port) {
  for (int i = 0; i < 1; i++) {
    dev_sync(dev_report, 1, dev_input, 1, ip, port, 100);
    usleep(1000000);
  }
}