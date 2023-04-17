#pragma once

#include "IRunwayContainer.hpp"
#include "Runway.hpp"

class Leopard : public IRunwayContainer<Runway> {
 private:
  int corenums;
  std::string ip;
  int port;

 public:
  Leopard(int corenums = 0);
  ~Leopard();
  void start(std::string ip, int port);
};