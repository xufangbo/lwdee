#include "src/Leopard.hpp"
#include "src/LaneClient.hpp"
#include "core/Exception.hpp"
#include "core/log.hpp"
#include "core/suspend.hpp"
#include "src/Epoll.hpp"
#include "src/LeopardConfig.hpp"
#include "src/Socket.hpp"
#include "testsuspend.hpp"
#include "test_input.hpp"
#include "test_qps.hpp"
#include "test_dev.hpp"

void do_test(std::string ip, int port) {
  // test_input(ip, port);
  // test_qps(ip, port);
  test_dev(ip, port);
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

  Leopard::instance.start(conf->cparallel);

  Stopwatch sw;

  do_test(ip, port);

  logger_info("test finished,eclapsed:%.3f", sw.stop());

  Leopard::instance.join();
  // Leopard::instance.stop();

  return 0;
}
