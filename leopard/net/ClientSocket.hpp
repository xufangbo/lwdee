#pragma once

#include <memory>
#include <atomic>
#include <map>
#include "Queue.hpp"
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

 public:
  SocketWaiter_t(uint32_t id)
      : id(id) {}

  void notify(WaitStatus status);
  uint32_t getId() { return id; }

  double wait(int timeout = 10);
};

typedef std::shared_ptr<SocketWaiter_t> SocketWaiter;

class Lane;
class ClientSocket : public Socket {
 private:
  std::map<uint64_t,SocketWaiter> waiters;
  Lane* lane = nullptr;

 public:
  ClientSocket(Lane* lane, Qps* qps)
      : lane(lane), Socket(qps) {}

  Lane* getLane() { return lane; }

  SocketWaiter crateWaiter(uint64_t id);
  SocketWaiter findWaiter(uint64_t id);
  bool hasWaiter();
};