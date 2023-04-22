#include "SocketWaiter.hpp"

#include "log.hpp"

void SocketWaiter_t::notify(WaitStatus status) {
  // leopard_trace("waiter notify [%d] status:%d", id, status);
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
  leopard_error("[%d] timeout in %lfs", id, timeout);
  return timeout;
}