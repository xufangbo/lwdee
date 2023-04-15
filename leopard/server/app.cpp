#include <iostream>
#include "net/Epoll.hpp"
#include "core/Exception.hpp"
#include "net/LeopardConfig.hpp"
#include "net/Socket.h"
#include "core/log.hpp"
#include "Leopard.hpp"
#include "order.hpp"

void init_logger(std::string nodeName);

int main() {
  init_logger("leopard");
  LeopardConfig::instance()->readConfig();
  auto ip = LeopardConfig::instance()->ip;
  // auto ip = "0.0.0.0";

  registService();

  Leopard leopard(LeopardConfig::instance()->procnums);
  leopard.start(ip, LeopardConfig::instance()->port);

  
}

void init_logger(std::string nodeName) {
  LogOption option;
  option.initalized = false;
  option.level = log_trace;
  strcpy(option.path, "./log");
  strcpy(option.name, nodeName.c_str());
  option.is_color = true;
  option.days = 10;
  option.writeFile = true;

  if (logger_initialize(option) != 0) {
    printf("log initialize error\n");
  } else {
    logger_info("-- app starting ... ");
  }
}