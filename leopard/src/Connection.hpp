#pragma once

#include <sys/epoll.h>
#include <list>
#include <memory>
#include <mutex>

#include "Bullet.hpp"
#include "Socket.hpp"
#include "Stopwatch.hpp"
#include "enums.hpp"

class Runway;
class ClientConnection;

class Connection {
 private:
  BulletList bullets;
  bool wr = true;
  std::mutex mut;

 protected:
  bool closed = false;
  Runway* runway;
  BufferStream* inStream;

 public:
  Socket* socket;
  uint64_t lastTime = 0;

 public:
  Connection(Runway* runway);
  Connection(int fd, Runway* runway);
  ~Connection();

  int fd();
  BufferStream* inputStream();
  void push(BufferStream* outputStream);
  void acceptRecive(epoll_event* evt);
  void acceptRequest(BufferStream* inputStream);
  void __acceptRequest(BufferStream* inputStream);
  void close(CloseType closeType);

  /**
   * @brief 是否正常发送
   *
   * @return true 正常发送，即使没有发送完下次还可以接着发送
   * @return false send返回-1且errno不为EAGAIN或EINTR，socket需要被清理
   */
  bool send(SendSource source);
  bool finished();
  void writable(bool wr);
  bool writable();
};
