#include "Antelope.hpp"
#include "LaneClient.hpp"
#include "core/Exception.hpp"
#include "core/log.hpp"
#include "core/suspend.hpp"
#include "net/Epoll.hpp"
#include "net/LeopardConfig.hpp"
#include "net/Socket.hpp"
#include "test.hpp"
#include "testsuspend.hpp"

TestReport testReport;

TestInput input_100_byte("100-bytes", 100);
TestInput input_1_K("1K", 1000);
TestInput input_10_K("10K", 10000);
TestInput input_100_K("100K", 100000);
TestInput input_1_M("1M", 1000000);
TestInput input_10_M("10M", 10000000);

TestSync test_sync;
TestAsync test_async;
TestLongConnect test_long;

void test_execute(Testor* testor, TestInput& input, int testSize, int parallel, float timeout, std::string ip, int port) {
  // testor->execute(testReport, testSize, input, parallel, ip, port,timeout);
  (*testor)(testReport, testSize, input, parallel, ip, port, timeout);
}

void test_by_testor(Testor* testor, int testSize, int parallel, std::string ip, int port) {
  // testor->execute(testReport, testSize, input, parallel, ip, port,timeout);
  (*testor)(testReport, testSize, input_100_byte, parallel, ip, port, 20);
  (*testor)(testReport, testSize, input_1_K, parallel, ip, port, 20);
  (*testor)(testReport, testSize, input_10_K, parallel, ip, port, 20);
  (*testor)(testReport, testSize, input_100_K, parallel, ip, port, 20);
  (*testor)(testReport, testSize, input_1_M, parallel, ip, port, 30);
  (*testor)(testReport, testSize, input_10_M, parallel, ip, port, 200);

  testReport.writeEmptyLine();
}

void test_by_input(TestInput& input, int testSize, int parallel, float timeout, std::string ip, int port) {
  test_sync(testReport, testSize, input, 1, ip, port, timeout);
  test_async(testReport, testSize, input, parallel, ip, port, timeout);
  test_long(testReport, testSize, input, parallel, ip, port, timeout);
  testReport.writeEmptyLine();
}

int main(int argc, char** argv) {
  for (int i = 0; i < 50; i++) {
    printf("\n");
  }
  read_log_config("client");
  auto* conf = LeopardConfig::instance();
  conf->readConfig();

  auto ip = conf->ip;
  auto port = conf->port;
  logger_trace("%s:%d", conf->ip.c_str(), conf->port);

  Antelope::instance.start(conf->cparallel);

  testReport.writeTitle();

  test_by_testor(&test_sync, 100, 1, ip, port);
  test_by_testor(&test_async, 100, 100, ip, port);
  test_by_testor(&test_long, 100, 1, ip, port);
  test_by_testor(&test_long, 100, 10, ip, port);

  // testReport.writeEmptyLine();

  // test_by_input(input_100_byte, 100, 10, 2, ip, port);
  // test_by_input(input_1_K, 100, 10, 2, ip, port);
  // test_by_input(input_10_K, 100, 10, 2, ip, port);
  // test_by_input(input_100_K, 100, 10, 2, ip, port);
  // test_by_input(input_1_M, 100, 10, 30, ip, port);
  // test_by_input(input_10_M, 10, 10, 200, ip, port);

  logger_info("test finished");

  Antelope::instance.join();
  // Antelope::instance.stop();

  return 0;
}
