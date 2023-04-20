#pragma once

#include <algorithm>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "Connection.hpp"
#include "Socket.hpp"

typedef std::shared_ptr<Connection> ConnectionPtr;

class Connections {
 private:
  std::mutex mut;
  std::map<int, ConnectionPtr> sockets;
  // std::vector<ConnectionPtr> sockets;

 private:
  std::mutex mut_task;
  std::list<Connection*> tasks;
  std::vector<Connection*> removedTasks;

  // std::queue<Socket*> adds;
  // std::queue<int> removes;

 private:
  bool* running;
  void run();
  void __run();
  Qps* qps;

 public:
  Connections(Qps* qps)
      : qps(qps) {
    // this->qps->waitings = [this]() { return 0; };
  }
  void start(bool* running);
  Connection* insert(Socket* s);
  Connection* find(int fd);
  void pushBullet(Connection* socket, BufferStreamPtr outputStream);
  void remove(int fd);
  void removeTasks();

  size_t size();
  void clear();
};