#pragma once

#include "TcpRequest.hpp"
#include "core/suspend.hpp"
#include "net/Socket.hpp"

class SocketClient;
typedef std::shared_ptr<SocketClient> SocketClientPtr;

class SocketClient {
 private:
  Socket* _socket;

 public:
  SocketClient(Socket* socket)
      : _socket(socket){};
  void invoke(std::string path, RequestInvoke request, RequestCallback callback);
  void invoke(std::string path, void* buffer, int len, RequestCallback callback);
#ifdef LEOPARD_SUSPEND
  await<BufferStream*> invoke(std::string path, void* buffer, int len);
#endif
  /**
   * @brief 等待调用返回
   *
   * @return 等待时间
   */
  double wait(int timeout = 20);
  Socket* socket();

  public:
  static SocketClientPtr create(const char* ip, int port);
};