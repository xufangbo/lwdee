#pragma once

#include <atomic>
#include <memory>

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
  uint32_t id;
  std::atomic<WaitStatus> status = WaitStatus::waiting;
  Stopwatch sw;

 public:
  SocketWaiter_t(uint32_t id) : id(id) { sw.start(); }

  void notify(WaitStatus status);
  uint32_t getId() { return id; }

  float wait(float timeout = 5);
};

typedef std::shared_ptr<SocketWaiter_t> SocketWaiter;