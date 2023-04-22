#include "ClientSocket.hpp"

#include "core/log.hpp"

void SocketWaiter_t::notify(WaitStatus status) {
  logger_trace("waiter notify [%d] status:%d", id, status);
  this->status = status;
}

float SocketWaiter_t::wait(float timeout) {
  while (sw.elapsed() < timeout) {
    if (status == WaitStatus::succeed) {
      return sw.stop();
    } else if (status == WaitStatus::timeout) {
      sw.stop();
      throw Exception("timeout", ZONE);
    } else if (status == WaitStatus::nohint) {
      return sw.stop();
      throw Exception("nohint", ZONE);
    } else {
      usleep(1000 * 100);
    }
  }
  // throw Exception("timeout", ZONE);
  // logger_error("timeout");
  logger_error("[%d] timeout in %lfs", id, timeout);
  return timeout;
}

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