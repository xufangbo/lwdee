#include <iostream>
#include "Leopard.hpp"
#include "core/log.hpp"
#include "net/LeopardConfig.hpp"
#include "order.hpp"

int main() {
  for (int i = 0; i < 50; i++) {
    printf("\n");
  }

  read_log_config("server");
  auto conf = LeopardConfig::instance();
  conf->readConfig();

  // auto ip = conf->ip;
  auto ip = "0.0.0.0";

  registService();

  Leopard leopard(ip, conf->port);
  leopard.start(ApplicationType::server, conf->sparallel);
  leopard.join();
}