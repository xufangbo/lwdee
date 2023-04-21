#pragma once

#include <map>
#include <memory>
#include <thread>

#include "net/Connection.hpp"
#include "net/Connections.hpp"
#include "net/Epoll.hpp"
#include "net/IRunway.hpp"
#include "net/Qps.hpp"

class Runway : public IRunway {
 private:
  std::string ip;
  int port;
  Socket* server = nullptr;

 private:
  void run() override;
  void acceptSocket(epoll_event* evt);
  void acceptEvent(epoll_event* evt) override;
  void __acceptRequest(Connection* connection, BufferStream* inputStream) override;

 public:
  Runway(int id, bool* running, std::string ip, int port);
  ~Runway();
  void start();
};