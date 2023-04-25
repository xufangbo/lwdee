#include "ClientConnection.hpp"

#include "Runway.hpp"
#include "log.hpp"

ClientConnection::ClientConnection(Runway* runway)
    : Connection(runway) {
  this->socket = new Socket(runway->qps());
  this->socket->setNonBlocking();
};

void ClientConnection::connect(std::string ip, int port) {
  Stopwatch sw;
  socket->connect(ip, port);
  if (sw.elapsed() > 1) {
    leopard_warn("long time to connect: %lfs", sw.elapsed());
  }
  socket->setNonBlocking();
}

ClientWaitor ClientConnection::crateWaiter(uint64_t id) {
  ClientWaitor waiter = std::make_shared<ClientWaitor_t>(id,this);
  waiters[id] = waiter;
  return waiter;
}

ClientWaitor ClientConnection::findWaiter(uint64_t id) {
  auto it = waiters.find(id);
  if (it == waiters.end()) {
    return nullptr;
  }

  return it->second;
}