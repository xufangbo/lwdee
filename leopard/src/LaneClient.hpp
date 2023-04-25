#pragma once

#include <atomic>
#include <mutex>
#include "ClientConnection.hpp"
#include "Socket.hpp"
#include "core/suspend.hpp"
#include "prot/TcpRequest.hpp"

class LaneClient;
typedef std::shared_ptr<LaneClient> LaneClientPtr;

class LaneClient {
 private:
  bool _autoClosed;
  bool closed = false;
  std::atomic<uint32_t> index;
  std::mutex mut;
  std::vector<ClientConnection*> connections;
  std::vector<ClientWaitor> waiters;

  void addConnection(ClientConnection* connection);

 public:
  LaneClient(){
    this->index = 0;
  }

  ~LaneClient();

 public:
  static LaneClientPtr create(std::string ip, int port, int parallel = 1);
  void onInvoked(ClientConnection* connection);
  void autoClosed(bool value){this->_autoClosed = value;}
  bool autoClosed(){return this->_autoClosed;}

 public:
  ClientWaitor invoke(std::string path, RequestInvoke request, RequestCallback callback);
  ClientWaitor invoke(std::string path, void* buffer, int len, RequestCallback callback);
#ifdef LEOPARD_SUSPEND
  await<BufferStream*> invoke(std::string path, void* buffer, int len);
#endif
  void close();
  void wait(float timeout = 5);

 private:
  ClientConnection* next();
};