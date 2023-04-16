#pragma

#include <memory>
#include <thread>

#include "SocketClient.hpp"
#include "TcpRequest.hpp"
#include "net/Epoll.hpp"
#include "net/Socket.h"
#include "net/Sockets.hpp"

class SocketScheduler {
 private:
  static Sockets clients;
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