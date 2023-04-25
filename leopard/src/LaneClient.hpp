#pragma once

#include <atomic>
#include "core/suspend.hpp"
#include "ClientSocket.hpp"
#include "Connection.hpp"
#include "Socket.hpp"
#include "prot/TcpRequest.hpp"

class LaneClient;
typedef std::shared_ptr<LaneClient> LaneClientPtr;

class LaneClient {
 private:
  std::atomic<uint32_t> index;
  std::vector<Connection*> connections;
  std::vector<SocketWaiter> waiters;

 public:
  LaneClient() {
    this->index = 0;
  }

  ~LaneClient() ;

 public:
  static LaneClientPtr create(const char* ip, int port, int parallel = 1);

 public:
  SocketWaiter invoke(std::string path, RequestInvoke request, RequestCallback callback);
  SocketWaiter invoke(std::string path, void* buffer, int len, RequestCallback callback);
#ifdef LEOPARD_SUSPEND
  await<BufferStream*> invoke(std::string path, void* buffer, int len);
#endif
  void close();
  void wait(float timeout = 5);
  std::vector<Connection*>* getConnections() { return &connections; }

 private:
  Connection* next();
};