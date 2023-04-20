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
  auto ip = conf->ip;
  auto port = conf->port;
  logger_trace("%s:%d", conf->ip.c_str(), conf->port);

  Antelope::instance.start();

  test_sync(1, input_small, ip, port);

  // test_sync(1000, input_small, ip, port);
  // test_sync(1000, input_large, ip, port);

  // test_async(1000, input_small, ip, port);
  // test_async(1000, input_small, ip, port);

  Antelope::instance.join();

  return 0;
}