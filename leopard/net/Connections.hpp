#pragma once

#include <algorithm>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "Connection.hpp"
#include "Socket.hpp"

class Connections {
 private:
  std::mutex mut;
  std::map<int, Connection*> items;

 private:
  std::mutex mut_task;
  std::list<Connection*> tasks;
  std::vector<Connection*> removedTasks;

 private:
  bool* running;
  void run();
  void __run();
  Qps* qps;

 public:
  Connections(Qps* qps) : qps(qps) {}
  void start(bool* running);
  Connection* insert(Socket* s);
  void remove(int fd);

 public:
  void pushBullet(Connection* socket, BufferStream* outputStream);
  void removeTasks();

  size_t size();
  void clear();
};