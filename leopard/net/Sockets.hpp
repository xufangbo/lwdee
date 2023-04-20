#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "SendTask.hpp"
#include "Socket.hpp"

class Sockets {
 private:
  std::mutex mut;
  std::map<int, std::shared_ptr<SendTask>> sockets;
  // std::vector<std::shared_ptr<SendTask>> sockets;

 private:
  bool* running;
  void run();
  void __run();

 public:
  void start(bool* running);
  SendTask* insert(Socket* s);
  SendTask* find(int fd);
  void pushBullet(Socket* socket, BufferStreamPtr outputStream);
  void remove(int fd);

  size_t size() { return sockets.size(); };
  void clear() { sockets.clear(); }
};