#pragma once

#include <memory>
#include <thread>
#include <map>

#include "net/Epoll.hpp"
#include "net/Socket.h"

class Runway {
 private:
  std::string ip;
  int port;
  Epoll* epoll = nullptr;
  int index;
  std::shared_ptr<Socket> server;
  std::thread thread;
  int _tps = 0;
  int _waits = 0;
  bool isET = true;
  bool epollOut = true;
  bool tracing = false;

 private:
  std::map<int,Socket*>* clients = nullptr;

 private:
  void run();
  void accept(epoll_event* evt);
  void io(epoll_event* evt);
  void recv(Socket* socket, epoll_event* evt);
  void handleRequest(Socket* socket);
  void doHandle(Socket* socket);
  void send(Socket* socket,BufferStream *outStream);
  void close(Socket* socket);

 public:
  Runway();
  ~Runway();
  int waits();
  int sockets();
  void start(std::string ip, int port, int index);
  void handleEvent(int i);
  void join();

 public:
  int tps();
};