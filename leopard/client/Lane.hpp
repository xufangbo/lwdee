#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include "SocketClient.hpp"
#include "TcpRequest.hpp"
#include "net/IRunway.hpp"

class Lane : public IRunway {
 private:
  void acceptEvent(epoll_event* evt) override;
  void doAcceptRequest(Socket* socket) override;

 public:
  Lane(int id, bool* running, SendTaskQueue* sendQueue);
  Socket* create(const char* ip, int port);
  void send(Socket* socket, BufferStreamPtr outputStream);
  void start();
  bool contains(int fd);
};