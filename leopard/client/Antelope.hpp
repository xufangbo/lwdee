#pragma

#include "Lane.hpp"
#include "SocketClient.hpp"
#include "net/IRunwayContainer.hpp"

class Antelope : public IRunwayContainer<Lane> {
 private:
  Antelope() {}

 public:
  static Antelope instance;

 public:
  void start();
  ClientSocket* create(const char* ip, int port);
  void send(Socket* socket, BufferStreamPtr outputStream);
  // bool contains(int fd);
};