#include <iostream>
#include "net/Epoll.hpp"
#include "core/Exception.hpp"
#include "net/LeopardConfig.hpp"
#include "net/Socket.hpp"
#include "core/log.hpp"
#include "Leopard.hpp"
#include "order.hpp"

int main() {
  read_log_config("server");
  LeopardConfig::instance()->readConfig();
  auto ip = LeopardConfig::instance()->ip;
  // auto ip = "0.0.0.0";

  registService();

  Leopard leopard(LeopardConfig::instance()->procnums);
  leopard.start(ip, LeopardConfig::instance()->port);
}