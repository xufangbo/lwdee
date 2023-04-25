#include "test_dev.hpp"

#include "core/log.hpp"

TestReport dev_report;
TestInput dev_input("1K", 1000);
TestSync dev_sync;

void task(std::string ip, int port);

void test_dev(std::string ip, int port) {
  dev_sync(dev_report, 100, dev_input, 2, ip, port, 100);
}