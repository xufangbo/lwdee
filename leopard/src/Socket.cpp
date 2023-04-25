#include "Socket.hpp"

#include <arpa/inet.h>
#include <fcntl.h>

#include "core/Exception.hpp"
#include "src/ProtocalFactory.hpp"
#include "string.h"

Socket::Socket(Qps* qps)
    : qps(qps) {
  _fd = ::socket(AF_INET, SOCK_STREAM, 0);  // AF_INET -> PF_INET
  if (-1 == _fd) {
    throw SocketException("socket create error", errno, ZONE);
  }
  this->_inputStream = ProtocalFactory::createStream();

  this->qps->opens++;
}

Socket::Socket(int fd, Qps* qps)
    : _fd(fd), qps(qps) {
  this->_inputStream = ProtocalFactory::createStream();
  this->qps->opens++;
}

Socket::~Socket() {
  if (_inputStream != nullptr) {
    delete _inputStream;
    _inputStream = nullptr;
  }
}

void Socket::bind(std::string ip, int port) {
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  if (strcmp(ip.c_str(), "0.0.0.0") == 0 || strcmp(ip.c_str(), "*.*.*.*")) {
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
  } else {
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
  }
  socklen_t len = sizeof(addr);
  if (-1 == ::bind(_fd, (sockaddr*)&addr, len)) {
    throw SocketException("socket bind error", errno, ZONE);
  }
}

void Socket::listen() {
  if (-1 == ::listen(_fd, 200)) {
    throw SocketException("socket listen error", errno, ZONE);
  }
}

int Socket::accept() {
  sockaddr_in addr;
  socklen_t len = sizeof(addr);

  int clientfd = ::accept(_fd, (sockaddr*)&addr, &len);
  if (clientfd == -1) {
    throw SocketException("socket accept error", errno, ZONE);
  }
  return clientfd;
}

void Socket::connect(std::string ip, int port) {
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip.c_str());
  if (-1 == ::connect(_fd, (sockaddr*)&addr, sizeof(addr))) {
    throw SocketException("socket connect error", errno, ZONE);
  }
}

ssize_t Socket::read(void* buf, size_t len) {
  int rc = ::read(_fd, buf, sizeof(buf));
  if (rc == -1) {
    if (errno == EINTR) {
      // 中断引起
      // donothing
      len = 0;
    } else if (errno = ECONNRESET) {
      throw SocketException("socket network error", errno, ZONE);  // 网络连接出了问题
    } else {
      throw SocketException("socket read error", errno, ZONE);
    }
  }
  return rc;
}

ssize_t Socket::write(void* buf, size_t len) {
  return ::write(_fd, buf, len);
}

ssize_t Socket::recv(void* buf, size_t len, int flags) {
  int rc = ::recv(_fd, buf, sizeof(buf), flags);
  if (rc == -1) {
    if (errno == EINTR) {
      // EINTR即errno为4，错误描述Interrupted system call
      return rc;
    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // EAGAIN:表示无数据可读，缓冲区已经为空，Resource temporarily unavailable
      return rc;
    } else if (errno == ECONNRESET) {
      // (104)Connection reset by peer
      return rc;
      // } else if (errno == ECONNREFUSED) {
      //   // (111)Connection refused
      //   return rc;
    } else {
      throw SocketException("socket recv error", errno, ZONE);
    }
  }
  return rc;
}

/**
 * send()函数非阻塞发送的时候，不一定能够全部发送出去，所以要检查send()函数的返回值
 * 当返回值小于len的时候，表明缓冲区中仍然有部分数据没有发送成功，这是需要重新发送剩余部分
 *
 * EINTR: Interrupted system call，在send和reci中意义不同，在send中表示这个系统调用被中断了
 * EAGAIN: 资源限制/不满足条件，导致返回值为EAGAIN,其实就是写缓冲区满了，下次再试
 * EWOULDBLOCK: 是EAGAIN的别名，在不同的操作系统下，有的有EAGAIN有的有EWOULDBLOCK，LINUX是两个宏都有
 */

ssize_t Socket::send(void* buf, size_t len) {
  return ::send(_fd, buf, len, 0);
}

ssize_t Socket::recvfrom(void* buf, size_t len, int flags, struct sockaddr* addr, socklen_t* addr_len) {
  int rc = ::recvfrom(_fd, buf, sizeof(buf), flags, addr, addr_len);
  if (rc == -1) {
    throw SocketException("socket recvfrom error", errno, ZONE);
  }
  return rc;
}

void Socket::close(CloseType closeType) {
  int rc = ::close(_fd);
  if (rc != -1) {
    if (closeType == CloseType::normal) {
      this->qps->closes++;
    } else {
      this->qps->errors++;
    }
  } else {
    this->qps->errors++;
    throw SocketException("socket close error", errno, ZONE);
  }
}

void Socket::shutdown(int how) {
  int rc = ::shutdown(_fd, how);
  if (rc == -1) {
    throw SocketException("socket close error", errno, ZONE);
  }
}

int Socket::getError() {
  int err;
  socklen_t socklen = sizeof(err);
  if (getsockopt(_fd, SOL_SOCKET, SO_ERROR, &err, &socklen) == -1) {
    throw SocketException("socket close error", errno, ZONE);
  }

  return err;
}

/**
 * @brief 设置句柄非阻塞
 *
 */
void Socket::setNonBlocking() {
  int flag = fcntl(_fd, F_GETFL);
  fcntl(_fd, F_SETFL, flag | O_NONBLOCK);
}

void Socket::reusePort() {
  int val = 1;
  if (setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val)) < 0) {
    throw SocketException("socket reuseport error", errno, ZONE);
  }
}

void Socket::setSendBuf(int size) {
  setsockopt(_fd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
}
void Socket::setReciveBuf(int size) {
  setsockopt(_fd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
}

int Socket::getSendBuf() {
  int sendbuf = 0;
  int len = sizeof(sendbuf);
  getsockopt(_fd, SOL_SOCKET, SO_SNDBUF, &sendbuf, (socklen_t*)&len);
  return sendbuf;
}
int Socket::getReciveBuf() {
  int recvbuf = 0;
  int len = sizeof(recvbuf);
  getsockopt(_fd, SOL_SOCKET, SO_RCVBUF, &recvbuf, (socklen_t*)&len);
  return recvbuf;
}