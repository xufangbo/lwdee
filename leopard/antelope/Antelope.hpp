#pragma

#include "Lane.hpp"
#include "LaneClient.hpp"
#include "net/IRunwayContainer.hpp"

class Antelope : public IRunwayContainer<Lane> {
 private:
  Antelope() {
    this->server = false;
  }
  void newInstance(int id, bool* running) override;

 public:
  static Antelope instance;

 public:
  Connection* create(const char* ip, int port);
  IRunway* selectRunway();
};