#include "Leopard.hpp"

#include "core/log.hpp"
#include "net/log.hpp"

Leopard::Leopard(std::string ip, int port)
    : ip(ip), port(port) {
  this->server = true;
}

void Leopard::newInstance(int id, bool* running) {
  auto runway = new Runway(id, running, ip, port);
  this->runways.push_back(runway);
}