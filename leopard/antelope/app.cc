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

int main(int argc, char** argv) {
  for (int i = 0; i < 100; i++) {
    printf("\n");
  }
  read_log_config("client");

  auto* conf = LeopardConfig::instance();
  conf->readConfig();
  auto ip = conf->ip;
  auto port = conf->port;
  logger_trace("%s:%d", conf->ip.c_str(), conf->port);

  Antelope::instance.start();

  // test_short_sync(1, input_small, ip, port);
  // test_short_async(1, input_large, ip, port);

  test_short_sync(1000, input_small, ip, port);  // ET: 11.41s  LT: 27.055
  // test_short_sync(1000, input_large, ip, port);    // ET: 131.306s LT: 166.388

  // test_short_async(1000, input_small, ip, port);  // ET:失败     LT: 0.771s -21 区别在于IRunway::__acceptEvent是否加锁
  // test_short_async(1000, input_large, ip, port);  // ET:失败     LT: 0.771s

  // test_long_sync(1000, input_small, 1, ip, port); // 3s
  // test_long_sync(1, input_large, 1, ip, port);

  // test_long_sync(1000, input_small, 100,ip, port);
  // test_long_sync(1000, input_large, 20,ip, port);

  // test_long_async(1000, input_small,100, ip, port);
  // test_long_async(1000, input_large,10, ip, port);

  Antelope::instance.join();

  return 0;
}