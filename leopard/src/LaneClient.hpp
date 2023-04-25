#pragma once

#include <atomic>
#include "ClientConnection.hpp"
#include "Socket.hpp"
#include "core/suspend.hpp"
#include "prot/TcpRequest.hpp"

class LaneClient;
typedef std::shared_ptr<LaneClient> LaneClientPtr;

class LaneClient {
 private:
  bool autoClose;
  bool closed = false;
  std::atomic<uint32_t> index;
  std::vector<ClientConnection*> connections;
  std::vector<ClientWaitor> waiters;
  

 public:
  LaneClient(bool autoClose = false)
      : autoClose(autoClose) {
    this->index = 0;
  }

  ~LaneClient();

 public:
  static LaneClientPtr create(std::string ip, int port, int parallel = 1);
  void onInvoked(ClientConnection* connection);

 public:
  ClientWaitor invoke(std::string path, RequestInvoke request, RequestCallback callback);
  ClientWaitor invoke(std::string path, void* buffer, int len, RequestCallback callback);
#ifdef LEOPARD_SUSPEND
  await<BufferStream*> invoke(std::string path, void* buffer, int len);
#endif
  void close();
  void wait(float timeout = 5);
  std::vector<ClientConnection*>* getConnections() { return &connections; }

 private:
  ClientConnection* next();
};