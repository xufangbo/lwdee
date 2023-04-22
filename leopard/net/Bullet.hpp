#pragma once

#include <list>
#include <memory>
#include "Queue.hpp"
#include "Socket.hpp"
#include "Stopwatch.hpp"

class Bullet {
 private:
  bool _finished = false;
  size_t pos = 0;
  BufferStream* outputStream = nullptr;
  Qps* qps;

 private:
  /**
   * @brief 下次发送数据，已经根据outputStream流根据pos做了偏移量
   */
  inline void* buffer() { return outputStream->buffer + pos; }

  /**
   * @brief 剩余字节数，即未发送完的字节数，待下次继续发送
   */
  inline size_t leftover() { return outputStream->size() - pos; }

  /**
   * @brief 发送完之后，如果没有发送完，要根据发送成功的记录数设置偏移量
   */
  inline void moveon(size_t len) { pos += len; }

 public:
  Bullet(BufferStream* outputStream, Qps* qps);

  ~Bullet();

  /**
   * @brief 是否正常发送
   *
   * @return true 正常发送，即使没有发送完下次还可以接着发送
   * @return false send返回-1且errno不为EAGAIN或EINTR，socket需要被清理
   */
  bool send(Socket* socket);

  bool finished() {
    return this->_finished;
  }

 private:
  static std::atomic<int> cout;
};

typedef Queue<Bullet*> BulletList;
