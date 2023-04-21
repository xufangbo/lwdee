#pragma once

#include "LaneClient.hpp"


class LaneClients {
 public:
  std::vector<LaneClientPtr> clients;
  
 public:
  LaneClientPtr create(const char* ip, int port);
  void wait();
  void close();
};