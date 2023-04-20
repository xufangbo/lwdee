#pragma once

#include <list>
#include <memory>
#include "Bullet.hpp"
#include "Socket.hpp"
#include "Stopwatch.hpp"

class Connection {
 private:
  bool _finished = false;
  BulletList bullets;

 public:
  Socket* socket;

 public:
  bool closed = false;

  Connection(Socket* socket);
  Connection(Socket* socket, BufferStreamPtr outputStream);

  void push(BufferStreamPtr outputStream);

  /**
   * @brief 是否正常发送
   *
   * @return true 正常发送，即使没有发送完下次还可以接着发送
   * @return false send返回-1且errno不为EAGAIN或EINTR，socket需要被清理
   */
  bool send();

  bool finished() {
    return this->_finished;
  }
};
