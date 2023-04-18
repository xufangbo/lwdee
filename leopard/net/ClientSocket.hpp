#pragma once

#include <memory>
#include <queue>
#include "Socket.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"

enum class WaitStatus : uint8_t {
  waiting = 0,
  succeed = 1,
  nohint = 2,
  timeout = 3
};

class SocketWaiter_t {
 private:
  WaitStatus status = WaitStatus::waiting;

 public:
  void notify(WaitStatus status) {
    this->status = status;
  }

  double wait(int timeout = 20) {
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
    throw Exception("timeout", ZONE);
  }
};

typedef std::shared_ptr<SocketWaiter_t> SocketWaiter;

class ClientSocket : public Socket {
 private:
  std::queue<SocketWaiter> waiters;

 public:
  ClientSocket(Qps* qps)
      : Socket(qps) {}

  void pushWaiter(SocketWaiter waiter) {
    waiters.push(waiter);
  }

  SocketWaiter popWaiter() {
    SocketWaiter waiter = waiters.front();
    waiters.pop();
    return waiter;
  }
};