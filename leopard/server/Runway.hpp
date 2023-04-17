#pragma once

#include <map>
#include <memory>
#include <thread>

#include "net/Epoll.hpp"
#include "net/IRunway.hpp"
#include "net/Qps.hpp"
#include "net/SendTask.hpp"
#include "net/Socket.hpp"
#include "net/Sockets.hpp"

class Runway : public IRunway {
 private:
  std::string ip;
  int port;
  std::shared_ptr<Socket> server;

 private:
  void accept(epoll_event* evt);

 public:
  Runway(int id, bool* running, SendTaskQueue* sendQueue);
  ~Runway();
  void start(std::string ip, int port);
  void handleEvent(int i);
  void join();

 public:
  Qps* qps();
};