#include "ClientSocket.hpp"

#include "core/log.hpp"


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
  logger_error("[%d] timeout in 30s",id);
  return timeout;
}

void ClientSocket::pushWaiter(SocketWaiter waiter) {
  waiters.push(waiter);
  // printf("< put waiter %d / %d\n", waiter->getId(), _fd);
}

SocketWaiter ClientSocket::popWaiter() {
  if (waiters.empty()) {
    // printf("pop waiter error %d \n", _fd);
    return nullptr;
  }
  SocketWaiter waiter = waiters.front();
  waiters.pop();
  // printf("> pop waiter %d / %d\n",waiter->getId(), _fd);
  return waiter;
}
