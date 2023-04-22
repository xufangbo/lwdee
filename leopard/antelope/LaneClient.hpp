#pragma once

#include <atomic>
#include "protocal/TcpRequest.hpp"
#include "core/suspend.hpp"
#include "net/ClientSocket.hpp"
#include "net/Connection.hpp"
#include "net/Socket.hpp"

class LaneClient;
typedef std::shared_ptr<LaneClient> LaneClientPtr;

class LaneClient {
 private:
  std::atomic<uint32_t> index;  
  std::vector<Connection*> connections;
  std::vector<SocketWaiter> waiters;

 public:
  LaneClient(){
    this->index = 0;
  }

 public:
  static LaneClientPtr create(const char* ip, int port, int parallel = 1);

 public:
  SocketWaiter invoke(std::string path, RequestInvoke request, RequestCallback callback);
  SocketWaiter invoke(std::string path, void* buffer, int len, RequestCallback callback);
#ifdef LEOPARD_SUSPEND
  await<BufferStream*> invoke(std::string path, void* buffer, int len);
#endif
  void close();
  void wait();
  std::vector<Connection*> *getConnections() { return &connections; }

 private:
  Connection* next();
};