#pragma once

#include <map>
#include <memory>
#include <thread>
#include "Connection.hpp"
#include "Epoll.hpp"
#include "Qps.hpp"

#define BUFFER_SIZE 1024

struct ServerInfo {
  std::string ip;
  int port;
  Socket* socket = nullptr;

  ~ServerInfo() {
  if (socket != nullptr) {
    delete socket;
    socket = nullptr;
  }
}
};

class ClientConnection;

class Runway {
 protected:
  int id;
  bool* running = nullptr;
  std::shared_ptr<Epoll> epoll;
  bool isET = true;
  bool isEOUT = false;
  ServerInfo server;

  uint32_t EVENTS_SERVER = 0;
  uint32_t EVENTS_NEW = 0;
  uint32_t EVENTS_IN = 0;
  uint32_t EVENTS_OUT = 0;

 private:
  Qps _qps;
  friend class Connection;

 protected:
  ApplicationType appType;
  std::thread thread;  
  void run();
  void __run();

 protected:
  char buf[BUFFER_SIZE] = {0};
  void acceptEvent(epoll_event* evt);
  void acceptSocket(epoll_event* evt);
  void __acceptEvent(epoll_event* evt);
  void acceptSend(Connection* connection);
  void gererateEnvents();

 public:
  Runway(int id,  bool* running);
  Runway(int id, bool* running,std::string ip,int port);
  ~Runway();
  void start();
  void epoll_mod_in(int fd, struct epoll_event* event, void* ptr);
  void epoll_mod_out(int fd, struct epoll_event* event, void* ptr);
  void epoll_del(int fd);
  Qps* qps();
  void join();
  size_t size() { return _qps.opens; }

  public:
  ClientConnection* create(std::string ip, int port);
};
