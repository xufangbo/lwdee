#include "Leopard.hpp"

#include <signal.h>
#include "core/log.hpp"
#include "sys/sysinfo.h"
#include "net/log.hpp"

Leopard::Leopard(int corenums)
    : corenums(corenums) {
  if (corenums <= 0) {
    this->corenums = get_nprocs();  // get_nprocs_conf();
  }
}

void Leopard::start(std::string ip, int port) {
  // https://blog.csdn.net/weixin_33196646/article/details/116730365
  signal(SIGPIPE, SIG_IGN);

  this->ip = ip;
  this->port = port;
  this->running = true;

  leopard_info("%s:%d,corenums:%d", ip.c_str(), port, corenums);

  sendQueue.start(&running);

  std::thread tps(&Leopard::qpsJob, this);
  tps.detach();

  for (int i = 0; i < corenums; i++) {
    auto runway = new Runway(i + 1, &running, &sendQueue);
    runway->start(ip, port);

    runways.push_back(runway);
  }
}

void Leopard::join() {
  for (auto runway : runways) {
    runway->join();
  }
}