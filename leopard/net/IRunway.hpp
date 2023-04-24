#pragma once

#include <map>
#include <memory>
#include <thread>
#include "Connection.hpp"
#include "Epoll.hpp"
#include "Qps.hpp"

#define BUFFER_SIZE 1024

class IRunway {
 protected:
  int id;
  bool* running = nullptr;
  std::shared_ptr<Epoll> epoll;
  bool isET = true;
  bool isEOUT = false;
  bool nonBlocking = false;
  char buf[BUFFER_SIZE] = {0};
  std::thread thread;
  uint32_t EVENTS_SERVER = 0;
  uint32_t EVENTS_NEW = 0;
  uint32_t EVENTS_IN = 0;
  uint32_t EVENTS_OUT = 0;

 protected:
  Qps _qps;

 protected:
  virtual void run();
  virtual void acceptEvent(epoll_event* evt) = 0;
  void __acceptEvent(epoll_event* evt);
  void acceptRecive(Connection* connection, epoll_event* evt);
  void acceptRequest(Connection* connection, BufferStream* inputStream);
  virtual void __acceptRequest(Connection* connection, BufferStream* inputStream) = 0;
  void acceptSend(Connection* connection);
  void gererateEnvents();

 public:
  IRunway(int id, bool* running);
  ~IRunway();
  void close(Connection* connection);
  Qps* qps();
  void join();
  size_t size() { return _qps.opens; }
  // void send(Connection* connection, BufferStream* outputStream);
};
