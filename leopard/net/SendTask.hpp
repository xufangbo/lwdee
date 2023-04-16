#pragma once

#include <list>
#include <mutex>
#include <vector>

#include "Socket.h"

class SendTask {
 private:
  bool _finished = false;
  uint64_t pos = 0;
  Socket *socket;
  BufferStreamPtr outputStream;

 private:
  inline void *buffer() { return outputStream->buffer + pos; }
  inline uint64_t leftover() { return outputStream->size() - pos; }
  inline void moveon(int size) { pos += size; }

 public:
  SendTask(Socket *socket, BufferStreamPtr outputStream)
      : socket(socket), outputStream(outputStream) {}

  void send();
  bool finished() { return this->_finished; }
};

class SendTaskQueue {
 private:
  bool running = false;
  std::mutex mut;
  std::list<SendTask *> list;
  std::vector<SendTask *> removes;

 private:
  void run();

 public:
  void push(Socket *socket, BufferStreamPtr outputStream);
  void start();
};