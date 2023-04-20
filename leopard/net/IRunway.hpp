#pragma once

#include <memory>
#include <thread>
#include "Epoll.hpp"
#include "LeopardProtocal.hpp"
#include "Qps.hpp"
#include "SendTaskQueue.hpp"
#include "Socket.hpp"
#include "Sockets.hpp"

class IRunway {
 protected:
  int id;
  bool* running = nullptr;

 protected:
  std::thread thread;
  std::shared_ptr<Epoll> epoll;
  bool isET = false;

 protected:
  Sockets sockets;
  SendTaskQueue* sendQueue;
  Qps _qps;

 protected:
  virtual void run();
  virtual void acceptEvent(epoll_event* evt) = 0;
  void __acceptEvent(epoll_event* evt);
  void acceptRecive(Socket* socket, epoll_event* evt);
  void acceptSend(Socket* socket, epoll_event* evt);
  void acceptRequest(Socket* socket,BufferStreamPtr inputStream);
  virtual void __acceptRequest(Socket* socket,BufferStreamPtr inputStream) = 0;
  ProtocalHeaderPtr parseRequest(BufferStream* inputStream);
  void close(Socket* socket);

 public:
  IRunway(int id, bool* running, SendTaskQueue* sendQueue);
  Qps* qps();
  void join();
  size_t size() { return sockets.size(); }
};
