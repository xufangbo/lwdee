#pragma

#include <algorithm>
#include <map>
#include <memory>
#include <mutex>
#include <thread>

#include "SocketClient.hpp"
#include "TcpRequest.hpp"
#include "net/Epoll.hpp"
#include "net/Socket.h"

class ClientSockets {
 private:
  std::mutex mut;
  std::vector<Socket*>* sockets = new std::vector<Socket*>();

 public:
  void insert(Socket* s) {
    mut.lock();
    sockets->push_back(s);
    mut.unlock();
  }
  Socket* find(int fd){
     mut.lock();
     auto it = std::find_if(sockets->begin(), sockets->end(),[&fd](Socket* i) { return i->fd() == fd; });
    if (it != sockets->end()) {
      return *it;
    }
    mut.unlock();
    return nullptr;
  }
  void remove(Socket* s) {
     mut.lock();
    auto it = std::find_if(sockets->begin(), sockets->end(),[s](Socket* i) { return i == s; });
    if (it != sockets->end()) {
      sockets->erase(it);
    }
    mut.unlock();
  }
};

class SocketScheduler {
 private:
  static ClientSockets clients;
  static std::shared_ptr<Epoll> epoll;
  static bool _running;
  static int waits;
  static int tps;
  static int unhandles;
  static std::thread tpsThread;
  static std::thread runningThread;
  static bool isET;

 public:
  static bool tracing;

 private:
  static void close(Socket* socket);
  static void tpsJob();
  static void handleEvent(epoll_event& evt);
  static void handleRequest(Socket* socket);
  static void recv(Socket* socket, epoll_event* evt);
  static void running();

 public:
  static SocketClientPtr newClient(const char* ip, int port);
  static void send(Socket* socket, void* buffer, size_t len);
  static void start();
  static void stop();
  static void join();
  static bool contains(int fd);
};