#pragma once

#include <signal.h>
#include <sys/sysinfo.h>

#include "Metrix.hpp"
#include "Runway.hpp"
#include "enums.hpp"
#include "log.hpp"

class Leopard {
 protected:
  int parallel = 1;
  bool running = false;
  std::vector<Runway*> runways;
  Metrix matrix;
  ApplicationType appType = ApplicationType::server;

 public:
  ~Leopard();
  void start(std::string ip, int port, int parallel = 1);
  void start(int parallel = 1);
  void stop();
  void join();

 public:
 static Leopard instance;
  Connection* create(const char* ip, int port);
};