#include "ClientSocket.hpp"

#include "core/log.hpp"

SocketWaiter ClientSocket::crateWaiter(uint64_t id) {
  SocketWaiter waiter = std::make_shared<SocketWaiter_t>(id);
  waiters[id] = waiter;
  return waiter;
}

SocketWaiter ClientSocket::findWaiter(uint64_t id) {
  auto it = waiters.find(id);
  if (it == waiters.end()) {
    return nullptr;
  }

  return it->second;
}