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
  std::thread thread;

 private:
  void run() override;
  void accept(epoll_event* evt);
  void handleEvent(epoll_event* evt) override;
  ProtocalHeaderPtr doHandle(Socket* socket) override;

 public:
  Runway(int id, bool* running, SendTaskQueue* sendQueue);
  void start(std::string ip, int port);
  void join();

 public:
  Qps* qps();
};