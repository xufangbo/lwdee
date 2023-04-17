#pragma once

#include <list>
#include <mutex>
#include <vector>

#include "Socket.hpp"

class SendTask {
 private:
  bool _finished = false;
  uint64_t pos = 0;
  Socket* socket;
  BufferStreamPtr outputStream;

 private:
  /**
   * @brief 下次发送数据，已经根据outputStream流根据pos做了偏移量
   */
  inline void* buffer() { return outputStream->buffer + pos; }

  /**
   * @brief 剩余字节数，即未发送完的字节数，待下次继续发送
   */
  inline uint64_t leftover() { return outputStream->size() - pos; }

  /**
   * @brief 发送完之后，如果没有发送完，要根据发送成功的记录数设置偏移量
   */
  inline void moveon(int size) { pos += size; }

 public:
  SendTask(Socket* socket, BufferStreamPtr outputStream)
      : socket(socket), outputStream(outputStream) {}

  /**
   * @brief 是否正常发送
   *
   * @return true 正常发送，即使没有发送完下次还可以接着发送
   * @return false send返回-1且errno不为EAGAIN或EINTR，socket需要被清理
   */
  bool send();

  /**
   * @brief 是否已经发送完成
   */
  bool finished() { return this->_finished; }
};

class SendTaskQueue {
 private:
  bool* running = nullptr;
  std::list<SendTask*> list;
  std::vector<SendTask*> removes;

 private:
  void run();
  void doRun();

 public:
  ~SendTaskQueue();
  void push(Socket* socket, BufferStreamPtr outputStream);
  void start(bool* running );
};