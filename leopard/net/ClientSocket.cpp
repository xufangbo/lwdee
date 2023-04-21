#include "ClientSocket.hpp"

#include "core/log.hpp"

void SocketWaiter_t::notify(WaitStatus status) {
  logger_trace("waiter notify [%d] status:%d", id, status);
  this->status = status;
}

double SocketWaiter_t::wait(int timeout) {
  Stopwatch sw;
  for (int i = 0; i < 100 * timeout; i++) {
    if (status == WaitStatus::succeed) {
      return sw.stop();
    } else if (status == WaitStatus::timeout) {
      sw.stop();
      throw Exception("timeout", ZONE);
    } else if (status == WaitStatus::nohint) {
      return sw.stop();
      throw Exception("nohint", ZONE);
    } else {
      usleep(1000000 / 100);
    }
  }
  // throw Exception("timeout", ZONE);
  // logger_error("timeout");
  logger_error("[%d] timeout in 30s", id);
  return timeout;
}

SocketWaiter ClientSocket::crateWaiter(int id) {
  SocketWaiter waiter = std::make_shared<SocketWaiter_t>(id);
  waiters.push(waiter);
  return waiter;
}

SocketWaiter ClientSocket::popWaiter() {
  if (waiters.empty()) {
    return nullptr;
  }
  SocketWaiter waiter = waiters.front();
  waiters.pop();
  return waiter;
}

bool  ClientSocket::hasWaiter(){
  return !waiters.empty();
}
