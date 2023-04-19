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
  void run() override;
  void acceptSocket(epoll_event* evt);
  void acceptEvent(epoll_event* evt) override;
  void __acceptRequest(Socket* socket, BufferStreamPtr inputStream) override;

 public:
  Runway(int id, bool* running, SendTaskQueue* sendQueue, std::string ip, int port);
  void start();

};