#include <iostream>
#include "Leopard.hpp"
#include "core/log.hpp"
#include "net/LeopardConfig.hpp"
#include "order.hpp"

int main() {
  for (int i = 0; i < 20; i++) {
    printf("\n");
  }

  read_log_config("server");
  LeopardConfig::instance()->readConfig();
  auto ip = LeopardConfig::instance()->ip;
  // auto ip = "0.0.0.0";

  registService();

  Leopard leopard(LeopardConfig::instance()->procnums);
  leopard.start(ip, LeopardConfig::instance()->port);
  leopard.join();
}