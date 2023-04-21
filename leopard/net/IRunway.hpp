#pragma once

#include <map>
#include <memory>
#include <thread>
#include "Connection.hpp"
#include "Connections.hpp"
#include "Epoll.hpp"
#include "Qps.hpp"

#define BUFFER_SIZE 1024

class IRunway {
 protected:
  int id;
  bool* running = nullptr;
  char buf[BUFFER_SIZE] = {0};

 protected:
  std::thread thread;
  std::shared_ptr<Epoll> epoll;
  bool isET = false;
  bool isEOUT = false;
  bool nonBlocking = false;

 protected:
  Connections* connections = new Connections(&_qps);
  Qps _qps;

 protected:
  virtual void run();
  virtual void acceptEvent(epoll_event* evt) = 0;
  void __acceptEvent(epoll_event* evt);
  void acceptRecive(Connection* connection, epoll_event* evt);
  void acceptRequest(Connection* connection, BufferStream* inputStream);
  virtual void __acceptRequest(Connection* connection, BufferStream* inputStream) = 0;

  void acceptSend(Connection* connection);
  
  inline uint32_t gererateEnvents();

 public:
  IRunway(int id, bool* running);
  ~IRunway();
  void close(Connection* connection);
  Qps* qps();
  void join();
  size_t size() { return connections->size(); }
  void addSendTask(Connection* connection, BufferStream* outputStream);
};
