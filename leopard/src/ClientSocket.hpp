#pragma once

#include <map>
#include "Socket.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "SocketWaiter.hpp"

class Runway;
class ClientSocket : public Socket {
 private:
  std::map<uint64_t, SocketWaiter> waiters;
  Runway* lane = nullptr;

 public:
  ClientSocket(Runway* lane, Qps* qps) : lane(lane), Socket(qps) {}

  Runway* getLane() { return lane; }

  SocketWaiter crateWaiter(uint64_t id);
  SocketWaiter findWaiter(uint64_t id);
};