#pragma once

#include <algorithm>
#include <mutex>
#include <vector>

#include "net/Socket.h"

class ClientSockets {
 private:
  std::mutex mut;
  std::vector<Socket*>* sockets = new std::vector<Socket*>();

 public:
  void insert(Socket* s);
  Socket* find(int fd);
  void remove(Socket* s);
};