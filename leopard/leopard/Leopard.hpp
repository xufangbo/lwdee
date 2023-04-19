#pragma once

#include "Runway.hpp"
#include "net/IRunwayContainer.hpp"

class Leopard : public IRunwayContainer<Runway> {
 private:
  int corenums;
  std::string ip;
  int port;

 public:
  Leopard(std::string ip, int port);

  void newInstance(int id, bool* running, SendTaskQueue* sendQueue) override;
};