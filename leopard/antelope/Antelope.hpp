#pragma

#include "Lane.hpp"
#include "SocketClient.hpp"
#include "net/IRunwayContainer.hpp"

class Antelope : public IRunwayContainer<Lane> {
 private:
  Antelope() {
    this->server = false;
  }
  void newInstance(int id, bool* running, SendTaskQueue* sendQueue) override ;

 public:
  static Antelope instance;

 public:
  ClientSocket* create(const char* ip, int port);
  void send(Socket* socket, BufferStreamPtr outputStream);
};