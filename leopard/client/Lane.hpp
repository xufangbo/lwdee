#pragma

#include <atomic>
#include <memory>
#include <thread>
#include "SocketClient.hpp"
#include "TcpRequest.hpp"
#include "net/Epoll.hpp"
#include "net/IRunway.hpp"
#include "net/Qps.hpp"
#include "net/SendTask.hpp"
#include "net/Socket.hpp"
#include "net/Sockets.hpp"

class Lane : public IRunway {
 private:
  void handleEvent(epoll_event& evt);
  ProtocalHeaderPtr doHandle(Socket* socket) override;

 public:
  Lane(int id, bool* running, SendTaskQueue* sendQueue);
  Socket* create(const char* ip, int port);
  void send(Socket* socket, BufferStreamPtr outputStream);
  void start();
  bool contains(int fd);
};