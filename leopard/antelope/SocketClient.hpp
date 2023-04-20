#pragma once

#include <atomic>
#include "TcpRequest.hpp"
#include "core/suspend.hpp"
#include "net/ClientSocket.hpp"
#include "net/Connection.hpp"
#include "net/Socket.hpp"

class SocketClient;
typedef std::shared_ptr<SocketClient> SocketClientPtr;

class SocketClient {
 private:
  int parallel = 1;
  std::atomic<uint32_t> index;
  std::atomic<uint32_t> waitId = 0;
  std::vector<Connection*> connections;

 public:
  SocketClient(std::vector<Connection*> connections)
      : connections(connections), parallel(connections.size()) {
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
  std::vector<Connection*>& getSockets() { return connections; }

 private:
  Connection* next();
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