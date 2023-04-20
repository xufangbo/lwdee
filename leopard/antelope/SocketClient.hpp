#pragma once

#include <atomic>
#include "TcpRequest.hpp"
#include "core/suspend.hpp"
#include "net/ClientSocket.hpp"
#include "net/SendTask.hpp"
#include "net/Socket.hpp"

class SocketClient;
typedef std::shared_ptr<SocketClient> SocketClientPtr;

class SocketClient {
 private:
  int parallel = 1;
  std::atomic<uint32_t> index;
  std::atomic<uint32_t> waitId = 0;
  std::vector<ClientSocket*> sockets;

 public:
  SocketClient(std::vector<ClientSocket*> sockets)
      : sockets(sockets), parallel(sockets.size()) {
    this->index = 0;
  }

 public:
  static SocketClientPtr create(const char* ip, int port, int parallel = 1);

 public:
  SocketWaiter invoke(std::string path, RequestInvoke request, RequestCallback callback);
  SocketWaiter invoke(std::string path, void* buffer, int len, RequestCallback callback);
#ifdef LEOPARD_SUSPEND
  await<BufferStream*> invoke(std::string path, void* buffer, int len);
#endif
  void close();
  void wait();
  std::vector<ClientSocket*>& getSockets() { return sockets; }

 private:
  ClientSocket* next();
};

class SocketClients {
 public:
  std::vector<SocketClientPtr> clients;
  std::vector<SocketWaiter> waiters;

 public:
  SocketClientPtr create(const char* ip, int port);
  void wait();
  void close();
};