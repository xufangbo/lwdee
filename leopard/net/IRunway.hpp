#pragma once

#include <map>
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
  std::map<int, std::shared_ptr<SendTask>> sendTasks;  //
  SendTaskQueue* sendQueue;
  Qps _qps;

 protected:
  virtual void run();
  virtual void acceptEvent(epoll_event* evt) = 0;
  void __acceptEvent(epoll_event* evt);
  void acceptRecive(Socket* socket, epoll_event* evt);
  void acceptRequest(Socket* socket, BufferStreamPtr inputStream);
  virtual void __acceptRequest(Socket* socket, BufferStreamPtr inputStream) = 0;
  ProtocalHeaderPtr parseRequest(BufferStream* inputStream);
  void close(Socket* socket);
  void acceptSend(Socket* socket, epoll_event* evt);
  void addSendTask(Socket* socket, BufferStreamPtr outputStream);

 public:
  IRunway(int id, bool* running, SendTaskQueue* sendQueue);
  Qps* qps();
  void join();
  size_t size() { return sockets.size(); }
};
