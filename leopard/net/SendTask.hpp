#pragma once

#include <mutex>
#include <list>

#include "Socket.h"

class SendTask {
 private:
  uint64_t pos = 0;

 public:
  Socket *socket;
  BufferStreamPtr outputStream;

 public:
  SendTask(Socket *socket, BufferStreamPtr outputStream)
      : socket(socket), outputStream(outputStream) {}

  bool finished() { return pos >= outputStream->size(); }
  void *buffer() { return outputStream->buffer + pos; }
  uint64_t leftover() { return outputStream->size() - pos; }
  void moveon(int size) { pos += size; }
};

class SendTaskQueue {
 private:
  bool running = false;
  std::mutex mut;
  std::list<SendTask*> list;

 private:
  void run();

 public:
  void push(Socket *socket, BufferStreamPtr outputStream);
  void start();
};