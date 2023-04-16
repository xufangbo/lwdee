#pragma once

#include <iostream>
#include <thread>
#include <vector>

#include "Runway.hpp"
#include "net/SendTask.hpp"

class Leopard {
 private:
  int corenums;
  std::string ip;
  int port;
  std::vector<Runway*> runways;
  SendTaskQueue sendQueue;

 public:
  Leopard(int corenums = 0);
  ~Leopard();
  void start(std::string ip, int port);

 private:
  std::thread tpsThread;
  void tpsJob();
};