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
TestInput input_10_K("10K", (10, 000));
TestInput input_100_K("100K", (100, 000));
TestInput input_1_M("1M", (1, 000, 000));
TestInput input_10_M("10M", (10, 000, 000));

void test_input(TestInput& input, std::string ip, int port) {
  test_sync(testReport, 1, input, ip, port, 1);         // ( 1682005236163 -> 10 -> 10 -> 12 ) <==> ( 0.010 + 0.000 + 0.002 = 0.012 )
  test_sync(testReport, 10, input, ip, port, 1);        // 10.282 leopard: 28.304  ( 1682094116562 -> 4294967267 -> 4294967268 -> 22 ) <==> ( 4294967.500 + 0.001 + 0.050 = 0.022 )
  test_async(testReport, 10, input, ip, port, 1);       // 0.259 , leopard: 10.03s
  test_long(testReport, 1000, input, 1, ip, port, 1);   // 0.749  leopard: 0.429
  test_long(testReport, 1000, input, 10, ip, port, 1);  // 0.158  leopard:
  test_long(testReport, 1000, input, 20, ip, port, 1);  // 0.158  leopard:
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

  Antelope::instance.start();

  testReport.writeTitle();

  test_input(input_100_byte, ip, port);
  test_input(input_10_K, ip, port);
  test_input(input_100_K, ip, port);
  test_input(input_1_M, ip, port);
  test_input(input_10_M, ip, port);

  // for (int i = 0; i < 100; i++) {
  //   test_sync(1000, input_100_byte, ip, port);  // 10.282
  //   // test_async(1000, input_100_byte, ip, port);  // 0.259 , offen  server or client segement failed
  //   // test_long(1000, input_100_byte, 10, ip, port);  // 0.020

  //   for (int t = 0; t < 30; t++) {
  //     printf("\n");
  //   }
  //   printf("%d ^^^^^^^^^^^^^^^^", i);
  //   sleep(1);
  // }

  Antelope::instance.join();
  // Antelope::instance.stop();

  return 0;
}
