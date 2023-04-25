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

class ClientConnection;
class LaneClient;

class ClientWaitor_t {
 private:
  uint32_t id;
  ClientConnection* connection;
  std::atomic<WaitStatus> status = WaitStatus::waiting;
  Stopwatch sw;

  public:
  LaneClient *client;

 public:
  ClientWaitor_t(uint32_t id, ClientConnection* connection)
      : id(id), connection(connection) { sw.start(); }

  void notify(WaitStatus status);
  uint32_t getId() { return id; }

  float wait(float timeout = 5);
};

typedef std::shared_ptr<ClientWaitor_t> ClientWaitor;