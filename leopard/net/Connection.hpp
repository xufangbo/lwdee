#pragma once

#include <list>
#include <memory>
#include <mutex>

#include "Bullet.hpp"
#include "Socket.hpp"
#include "Stopwatch.hpp"
#include "enums.hpp"

class Connection {
 private:
  BulletList bullets;
  Qps* qps;
  bool wr = true;
  std::mutex mut;

 public:
  Socket* socket;

 public:
  bool closed = false;

  Connection(Socket* socket, Qps* qps);
  ~Connection();

  void push(BufferStream* outputStream);

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
