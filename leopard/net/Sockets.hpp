#pragma once

#include <algorithm>
#include <mutex>
#include <vector>

#include "net/Socket.h"

class Sockets {
 private:
  std::mutex mut;
  std::vector<Socket*>* sockets = new std::vector<Socket*>();

 public:
  ~Sockets() {
    if (sockets != nullptr) {
      sockets->clear();
      delete sockets;
      sockets = nullptr;
    }
  }
  void insert(Socket* s);
  Socket* find(int fd);
  void remove(Socket* s);
  size_t size() { return sockets->size(); };
  void clear() { sockets->clear(); }
};