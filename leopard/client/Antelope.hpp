#pragma

#include "IRunwayContainer.hpp"
#include "Lane.hpp"

class Antelope : public IRunwayContainer<Lane> {
 private:
  Antelope() {}
  ~Antelope();

 public:
  void start();
  SocketClientPtr create(const char* ip, int port);

 public:
  static Antelope instance;
};