#pragma once

#include <memory>
#include <thread>
#include "Epoll.hpp"
#include "LeopardProtocal.hpp"
#include "Qps.hpp"
#include "SendTask.hpp"
#include "Socket.hpp"
#include "Sockets.hpp"

class IRunway {
 protected:
  int id;
  bool* running = nullptr;

 protected:
  std::thread thread;
  std::shared_ptr<Epoll> epoll;
  bool isET = true;
  bool epollOut = true;

 protected:
  Sockets sockets;
  SendTaskQueue* sendQueue;
  Qps _qps;

 protected:
  virtual void run();
  virtual void acceptEvent(epoll_event* evt) = 0;
  void acceptRecive(epoll_event* evt);
  void doAcceptRecive(Socket* socket, epoll_event* evt);
  void acceptRequest(Socket* socket);
  virtual void doAcceptRequest(Socket* socket) = 0;
  ProtocalHeaderPtr parseRequest(Socket* socket);
  void close(Socket* socket);

 public:
  IRunway(int id, bool* running, SendTaskQueue* sendQueue);
  Qps* qps();
  void join();
  size_t size() { return sockets.size(); }
};
