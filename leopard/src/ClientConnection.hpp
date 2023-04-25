#pragma once

#include <map>
#include "Connection.hpp"
#include "Socket.hpp"
#include "ClientWaitor.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"

class ClientConnection : public Connection {
 private:
  std::map<uint64_t, ClientWaitor> waiters;

 public:
  ClientConnection(Runway* runway);

  void connect(std::string ip, int port);
  ClientWaitor crateWaiter(uint64_t id);
  ClientWaitor findWaiter(uint64_t id);
};