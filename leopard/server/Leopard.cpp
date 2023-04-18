#include "Leopard.hpp"

#include "core/log.hpp"
#include "net/log.hpp"

Leopard::Leopard(std::string ip, int port)
    : ip(ip), port(port) {
}

void Leopard::newInstance(int id, bool* running, SendTaskQueue* sendQueue) {
  auto runway = new Runway(id, running, sendQueue, ip, port);
  this->runways.push_back(runway);
}