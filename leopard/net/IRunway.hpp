#pragma once

#include <memory>
#include "Epoll.hpp"
#include "Qps.hpp"
#include "SendTask.hpp"
#include "Socket.hpp"
#include "Sockets.hpp"

class IRunway {
 protected:
  int id;
  bool* running = nullptr;

 protected:
  std::shared_ptr<Epoll> epoll;
  bool isET = true;
  bool epollOut = true;

 protected:
  Sockets sockets;
  SendTaskQueue* sendQueue;
  Qps _qps;

 protected:
  void run();
  void io(epoll_event* evt);
  void recv(Socket* socket, epoll_event* evt);
  void handleRequest(Socket* socket);
  void doHandle(Socket* socket);
  void close(Socket* socket);

 public:
  void stop();
};
