#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "BufferStream.hpp"
#include "Qps.hpp"
#include "enums.hpp"

class Connection;
class Bullet;

class Socket {
  friend class Connection;
  friend class Bullet;

 protected:
  int _fd;
  BufferStream* _inputStream;
  Qps* qps = nullptr;

 protected:
  BufferStream* inputStream() { return _inputStream; }
  ssize_t read(void* buf, size_t len);
  ssize_t write(void* buf, size_t len);
  ssize_t recv(void* buf, size_t len, int flags);
  ssize_t send(void* buf, size_t len);
  ssize_t recvfrom(void* buf, size_t len, int flags, struct sockaddr* addr, socklen_t* addr_len);

  void close(CloseType closeType);
  void shutdown(int how = SHUT_RDWR);

  int getError();

 public:
  Socket(Qps* qps);
  Socket(int fd, Qps* qps);
  ~Socket();
  int& fd() { return _fd; }

  void bind(std::string ip, int port);
  void listen();
  int accept();
  void connect(std::string ip, int port);

  void setNonBlocking();
  void reusePort();
  void setSendBuf(int size);
  void setReciveBuf(int size);
  int getSendBuf();
  int getReciveBuf();
};