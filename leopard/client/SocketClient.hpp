#pragma once

#include "TcpRequest.hpp"
#include "net/Socket.h"
#include "core/suspend.hpp"

class SocketClient;
typedef std::shared_ptr<SocketClient> SocketClientPtr;

class SocketClient {
 private:
  Socket* _socket;

 public:
  SocketClient(Socket* socket) : _socket(socket){};
  void invoke(std::string path, RequestInvoke request, RequestCallback callback);
  void invoke(std::string path, void* buffer,int len, RequestCallback callback);
  await<BufferStream*> invoke(std::string path, void* buffer,int len);
  void wait();
  Socket* socket();
};