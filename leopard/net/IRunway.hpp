#pragma once

#include <map>
#include <memory>
#include <thread>
#include "Epoll.hpp"
#include "LeopardProtocal.hpp"
#include "Qps.hpp"
#include "Connection.hpp"
#include "Connections.hpp"

class IRunway {
 protected:
  int id;
  bool* running = nullptr;

 protected:
  std::thread thread;
  std::shared_ptr<Epoll> epoll;
  bool isET = false;

 protected:
  std::mutex sktlock;
  Connections* connections = new Connections(&_qps);
  Qps _qps;

 protected:
  virtual void run();
  virtual void acceptEvent(epoll_event* evt) = 0;
  void __acceptEvent(epoll_event* evt);
  void acceptRecive(Connection* connection, epoll_event* evt);
  void acceptRequest(Connection* connection, BufferStreamPtr inputStream);
  virtual void __acceptRequest(Connection* connection, BufferStreamPtr inputStream) = 0;
  ProtocalHeaderPtr parseRequest(BufferStream* inputStream);

  void acceptSend(Connection* connection);
  void addSendTask(Connection* connection, BufferStreamPtr outputStream);

 public:
  IRunway(int id, bool* running);
  ~IRunway() ;
  void close(Connection* connection);
  Qps* qps();
  void join();
  size_t size() { return connections->size(); }
};
