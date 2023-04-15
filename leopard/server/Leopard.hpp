#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include "Runway.hpp"

class Leopard {
 private:
  int corenums;
  std::string ip;
  int port;
  std::vector<Runway*> runways;

 public:
  Leopard(int corenums = 0);
  ~Leopard();
  void start(std::string ip, int port);

  private:
   std::thread tpsThread;
   void tpsJob();
};