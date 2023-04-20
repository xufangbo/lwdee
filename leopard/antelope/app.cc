#include "Antelope.hpp"
#include "SocketClient.hpp"
#include "core/Exception.hpp"
#include "core/log.hpp"
#include "core/suspend.hpp"
#include "net/Epoll.hpp"
#include "net/LeopardConfig.hpp"
#include "net/Socket.hpp"
#include "test.hpp"
#include "testsuspend.hpp"

int main(int argc, char** argv) {
  for (int i = 0; i < 30; i++) {
    printf("\n");
  }
  read_log_config("client");

  auto* conf = LeopardConfig::instance();
  conf->readConfig();
  logger_trace("%s:%d", conf->ip.c_str(), conf->port);

  Antelope::instance.start();

  // test_1000_small_short_sync(conf->ip, conf->port);
  // test_1000_small_short_async(conf->ip, conf->port);
  test_1000_large_short_sync(conf->ip, conf->port);
  // test_1000_large_short_async(conf->ip, conf->port);
  Antelope::instance.join();

  return 0;
}