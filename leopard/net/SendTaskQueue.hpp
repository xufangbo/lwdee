#pragma once

#include <list>
#include <mutex>
#include <vector>

#include "SendTask.hpp"

class SendTaskQueue {
 private:
  int id;
  bool* running = nullptr;
  std::list<SendTask*> list;
  std::vector<SendTask*> removes;
  std::mutex mut;

 private:
  void run();
  void doRun();

 public:
  SendTaskQueue(int id)
      : id(id) {}
  ~SendTaskQueue();
  void push(Socket* socket, BufferStreamPtr outputStream);
  void push(SendTask* task);
  void start(bool* running);
  void remove();
};