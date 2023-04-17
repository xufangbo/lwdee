#pragma

#include "Lane.hpp"
#include "net/IRunwayContainer.hpp"

class Antelope : public IRunwayContainer<Lane> {
 private:
  ~Antelope();

 public:
  void start();
  SocketClientPtr create(const char* ip, int port);

 public:
  static Antelope instance;
};