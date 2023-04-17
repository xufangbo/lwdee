#pragma once

#include <memory>
#include "Epoll.hpp"
#include "Qps.hpp"
#include "SendTask.hpp"
#include "Socket.hpp"
#include "Sockets.hpp"
#include "LeopardProtocal.hpp"

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
  virtual void run();
  virtual void handleEvent(epoll_event* evt);
  virtual ProtocalHeaderPtr doHandle(Socket* socket);
  void io(epoll_event* evt);
  void recv(Socket* socket, epoll_event* evt);
  void handleRequest(Socket* socket);
  void close(Socket* socket);

 public:
  IRunway(int id, bool* running, SendTaskQueue* sendQueue);
  void stop();
  Qps* qps();
  size_t size(){return sockets.size();}
};
