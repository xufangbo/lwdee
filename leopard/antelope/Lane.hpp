#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include "LaneClient.hpp"
#include "TcpRequest.hpp"
#include "net/IRunway.hpp"
#include "net/Connection.hpp"

class Lane : public IRunway {
 private:
  void acceptEvent(epoll_event* evt) override;
  void __acceptRequest(Connection* socket,BufferStreamPtr inputStream) override;

 public:
  Lane(int id, bool* running);
  Connection* create(const char* ip, int port);
  void send(Connection* socket, BufferStreamPtr outputStream);
  void start();
  // bool contains(int fd);
  // void close(int fd);
};