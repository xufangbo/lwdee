#pragma once

#include <map>
#include "Socket.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "SocketWaiter.hpp"

class Lane;
class ClientSocket : public Socket {
 private:
  std::map<uint64_t, SocketWaiter> waiters;
  Lane* lane = nullptr;

 public:
  ClientSocket(Lane* lane, Qps* qps) : lane(lane), Socket(qps) {}

  Lane* getLane() { return lane; }

  SocketWaiter crateWaiter(uint64_t id);
  SocketWaiter findWaiter(uint64_t id);
};