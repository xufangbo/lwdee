#include <iostream>
#include "core/log.hpp"
#include "src/Leopard.hpp"
#include "src/LeopardConfig.hpp"
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

  Leopard leopard;
  leopard.start(ip, conf->port, conf->sparallel);
  leopard.join();
}