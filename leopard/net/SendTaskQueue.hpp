#pragma once

#include <list>
#include <mutex>
#include <vector>

#include "Qps.hpp"
#include "SendTask.hpp"

class SendTaskQueue {
 private:
  int id;
  bool* running = nullptr;
  std::list<SendTask*> list;
  std::vector<SendTask*> removes;
  Qps _qps;
  // std::mutex mut;

 private:
  void run();
  void doRun();

 public:
  SendTaskQueue(int id)
      : id(id), _qps(id) {
    this->_qps.waitings = [this]() { return 0; };
  }
  ~SendTaskQueue();
  void push(Socket* socket, BufferStreamPtr outputStream);
  void push(SendTask* task);
  void start(bool* running);
  void remove();
  Qps* qps() {
    return &_qps;
  }
};