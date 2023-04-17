#pragma once

#include "Runway.hpp"
#include "net/IRunwayContainer.hpp"

class Leopard : public IRunwayContainer<Runway> {
 private:
  int corenums;
  std::string ip;
  int port;

 public:
  Leopard(int corenums = 0);
  void start(std::string ip, int port);
  void join();
};