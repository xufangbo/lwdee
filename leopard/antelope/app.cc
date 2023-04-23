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

void test_input(TestInput& input, int testSize, float timeout, std::string ip, int port) {
  test_sync(testReport, 1, input, 1, ip, port, timeout);  // ( 1682005236163 -> 10 -> 10 -> 12 ) <==> ( 0.010 + 0.000 + 0.002 = 0.012 )
  // test_sync(testReport, testSize, input,testSize, ip, port, timeout);      // 10.282 leopard: 28.304  ( 1682094116562 -> 4294967267 -> 4294967268 -> 22 ) <==> ( 4294967.500 + 0.001 + 0.050 = 0.022 )
  // test_async(testReport, testSize, input,testSize, ip, port, timeout);     // 0.259 , leopard: 10.03s
  test_long(testReport, testSize, input, 1, ip, port, timeout);   // 0.749  leopard: 0.429
  test_long(testReport, testSize, input, 10, ip, port, timeout);  // 0.158  leopard:
  // test_long(testReport, testSize, input, 20, ip, port, timeout);  // 0.158  leopard:
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

  test_execute(&test_sync, input_100_byte, 100, 1, 30, ip, port);
  test_execute(&test_sync, input_1_K, 100, 1, 30, ip, port);
  test_execute(&test_sync, input_10_K, 100, 1, 30, ip, port);
  test_execute(&test_sync, input_100_K, 100, 1, 30, ip, port);
  test_execute(&test_sync, input_1_M, 100, 1, 30, ip, port);
  test_execute(&test_sync, input_10_M, 10, 1, 30, ip, port);

  test_execute(&test_async, input_100_byte, 100, 100, 30, ip, port);
  test_execute(&test_async, input_1_K, 100, 100, 30, ip, port);
  test_execute(&test_async, input_10_K, 100, 100, 30, ip, port);
  test_execute(&test_async, input_100_K, 100, 100, 30, ip, port);
  test_execute(&test_async, input_1_M, 100, 100, 30, ip, port);
  test_execute(&test_async, input_10_M, 10, 100, 30, ip, port);

  test_execute(&test_long, input_100_byte, 100, 1, 30, ip, port);
  test_execute(&test_long, input_1_K, 100, 1, 30, ip, port);
  test_execute(&test_long, input_10_K, 100, 1, 30, ip, port);
  test_execute(&test_long, input_100_K, 100, 1, 30, ip, port);
  test_execute(&test_long, input_1_M, 100, 1, 30, ip, port);
  test_execute(&test_long, input_10_M, 10, 1, 30, ip, port);

  test_execute(&test_long, input_100_byte, 100, 10, 30, ip, port);
  test_execute(&test_long, input_1_K, 100, 10, 30, ip, port);
  test_execute(&test_long, input_10_K, 100, 10, 30, ip, port);
  test_execute(&test_long, input_100_K, 100, 10, 30, ip, port);
  test_execute(&test_long, input_1_M, 100, 10, 30, ip, port);
  test_execute(&test_long, input_10_M, 10, 10, 30, ip, port);

  // test_input(input_100_byte, 100, 30, ip, port);
  // test_input(input_1_K, 100, 30, ip, port);
  // test_input(input_10_K, 100, 30, ip, port);
  // test_input(input_100_K, 100, 100, ip, port);
  // test_input(input_1_M, 10, 200, ip, port);
  // test_input(input_10_M, 10, 500, ip, port);

  logger_info("test finished");

  Antelope::instance.join();
  // Antelope::instance.stop();

  return 0;
}
