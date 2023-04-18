#pragma once

#include "TcpRequest.hpp"
#include "core/suspend.hpp"
#include "net/ClientSocket.hpp"
#include "net/SendTask.hpp"
#include "net/Socket.hpp"

class SocketClient;
typedef std::shared_ptr<SocketClient> SocketClientPtr;

class SocketClient {
 private:
  ClientSocket* socket;

 public:
  SocketClient(ClientSocket* socket)
      : socket(socket){};

 public:
  static SocketClientPtr create(const char* ip, int port);

 public:
  SocketWaiter invoke(std::string path, RequestInvoke request, RequestCallback callback);
  SocketWaiter invoke(std::string path, void* buffer, int len, RequestCallback callback);
#ifdef LEOPARD_SUSPEND
  await<BufferStream*> invoke(std::string path, void* buffer, int len);
#endif
  void close();
};