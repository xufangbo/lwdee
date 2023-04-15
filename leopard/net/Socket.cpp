#include "Socket.h"

#include <arpa/inet.h>
#include <fcntl.h>

#include "core/Exception.hpp"
#include "net/ProtocalFactory.hpp"
#include "string.h"

Socket::Socket() {
  _fd = ::socket(AF_INET, SOCK_STREAM, 0);  // AF_INET -> PF_INET
  if (-1 == _fd) {
    throw SocketException("socket create error", errno, ZONE);
  }
  this->_inputStream = ProtocalFactory::createStream();
}

Socket::Socket(int fd) : _fd(fd) {
  this->_inputStream = ProtocalFactory::createStream();
}

void Socket::bind(const char* ip, int port) {
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  if (strcmp(ip, "0.0.0.0") == 0 || strcmp(ip, "*.*.*.*")) {
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
  } else {
    addr.sin_addr.s_addr = inet_addr(ip);
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

void Socket::connect(const char* ip, int port) {
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);
  if (-1 == ::connect(_fd, (sockaddr*)&addr, sizeof(addr))) {
    throw SocketException("socket connect error", errno, ZONE);
  }
}

/**
 * read是阻塞读数据
 * 数据在不超过指定的长度的时候有多少读多少，没有数据则会一直等待。
 * 所以一般情况下：我们读取数据都需要采用循环读的方式读取数据，因为一次read
 * 完毕不能保证读到我们需要长度的数据，read
 * 完一次需要判断读到的数据长度再决定是否还需要再次读取。
 * 会发生返回值比指定长度短的情况
 *
 * > 0 : 表示实际所读的字节数
 * = 0 : 表示已经读到文件的结束了
 * < 0 : 表示出现了错误.如果错误为EINTR说明读是由中断引起的,
 * 如果是ECONNREST表示网络连接出了问题.
 */
ssize_t Socket::read(void* buf, size_t len) {
  // 字节流套接口(如tcp套接口)上的read和write函数所表现的行为不同于通常的文件IO。字节流套接口上的读或写、输入或输出的字节数可能比要求的数量少。
  // 但这不是错误状况，原因是内核中套接口的缓冲区可能已达到了极限。此时所需的是调用者再次调用read或write函数，以输入或输出剩余的字节。
  // 可以使用readn函数来实现循环读取以解决这个问题

  int rc = ::read(_fd, buf, sizeof(buf));
  if (rc == -1) {
    if (errno == EINTR) {
      // 中断引起
      // donothing
      len = 0;
    } else if (errno = ECONNRESET) {
      throw SocketException("socket network error", errno,
                            ZONE);  // 网络连接出了问题
    } else {
      throw SocketException("socket read error", errno, ZONE);
    }
  }
  return rc;
}

/**
 * 在进行TCP协议传输的时候，要注意数据流传输的特点，recv和send不一定是一一对应的（一般情况下是一一对应），
 * 也就是说并不是send一次，就一定recv一次就接收完，有可能send一次，recv多次才接收完，也可能send多次，一次recv就接收完了。
 * TCP协议会保证数据的有序完整的传输，但是如何去正确完整的处理每一条信息，是程序员的事情。
 *
 * 例如：服务器在循环recv，recv的缓冲区大小为100byte，客户端在循环send，
 * 每次send
 * 6byte数据，则recv每次收到的数据可能为6byte，12byte，18byte，这是随机的，编程的时候注意正确的处理。
 */
ssize_t Socket::write(void* buf, size_t len) { return ::write(_fd, buf, len); }

// https://blog.csdn.net/dyzhen/article/details/84992576
// read(sockfd, buff, buff_size);
// write(sockfd, buff, buff_size);
// recv(sockfd, buff, buff_size,MSG_WAITALL); //阻塞模式接收
// send(scokfd, buff, buff_size,MSG_WAITALL); //阻塞模式发送
// recv(sockfd, buff, buff_size,MSG_DONTWAIT); //非阻塞模式接收
// send(scokfd, buff, buff_size,MSG_DONTWAIT); //非阻塞模式发送
// recv(sockfd, buff, buff_size,0);
// send(scokfd, buff, buff_size,0);

/**
 * flags 参数
 * 0            : recv和read一样
 * MSG_DONTROUTE:
 这个标志告诉IP.目的主机在本地网络上面,没有必要查找表.这个标志一般用网络诊断和路由程序里面.
 * MSG_OOB      : 接受或者发送带外数据
 * MSG_PEEK     :
 是recv函数的使用标志,表示只是从系统缓冲区中读取内容,而不清除系统缓冲区的内容.这样下次读的时候,仍然是一样的内容.一般在有多个进程读写数据时可以使用这个标志.
 * MSG_WAITALL  :
 recv函数的使用标志,表示等到所有的信息到达时才返回.使用这个标志的时候recv回一直阻塞,直到指定的条件满足,或者是发生了错误.
 *                     1)当读到了指定的字节时,函数正常返回.返回值等于len
 *                     2)当读到了文件的结尾时,函数正常返回.返回值小于len
 *                     3)当操作发生错误时,返回-1,且设置错误为相应的错误号(errno)
 *
 * MSG_NOSIGNAL : is a flag used by send a SIGPIPE signal on errors on stream
 * oriented sockets when the other end breaks the connection. The EPIPE error is
 * still returned as normal。 Though it is in some Berkely sockets APIs (notably
 * Linux) it does not exist in what some refer to as the reference
 * implementation, FreeBSD, which instead uses a socket option. SO_NOSIGPIPE :
 * 对于服务器端，我们可以使用这个标志。目的是不让其发送SIG_PIPE信号，导致程序退出。
 *
    成功执行时，返回接收到的字节数。
    另一端已关闭则返回0。
    失败返回-1，errno被设为以下的某个值 ：
    EAGAIN：套接字已标记为非阻塞，而接收操作被阻塞或者接收超时
    EBADF：sock不是有效的描述词
    ECONNREFUSE：远程主机阻绝网络连接
    EFAULT：内存空间访问出错
    EINTR：操作被信号中断
    EINVAL：参数无效
    ENOMEM：内存不足
    ENOTCONN：与面向连接关联的套接字尚未被连接上
    ENOTSOCK：sock索引的不是套接字 当返回值是0时，为正常关闭连接；

    天那，要程序员自己代码中设置结束符来判断结束！
 */
ssize_t Socket::recv(void* buf, size_t len, int flags) {
  int rc = ::recv(_fd, buf, sizeof(buf), flags);
  if (rc == -1) {
    if (errno == EINTR) {
      // EINTR即errno为4，错误描述Interrupted system call
      return rc;
    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // EAGAIN的意思是：E_again,要再次发送
      // 在VxWorks和Windows上，EAGAIN的名字叫做EWOULDBLOCK
      return rc;
    } else {
      throw SocketException("socket recv error", errno, ZONE);
    }
  }
  return rc;
}

/**
 * send()函数发送的时候，不一定能够全部发送出去，所以要检查send()函数的返回值
 * 当返回值小于len的时候，表明缓冲区中仍然有部分数据没有发送成功，这是需要重新发送剩余部分
 *
 * 如果返回-1，表示错误
 */

ssize_t Socket::send(void* buf, size_t len) {
  // 当应用程序在socket中设置O_NONBLOCK属性后，如果发送缓存被占满，send就会返回EAGAIN或EWOULDBLOCK的错误。
  // 在将socket设置O_NONBLOCK属性后，通过socket发送一个100K大小的数据，
  // 第一次成功发送了13140数据，之后继续发送并未成功，errno数值为EAGAIN错误。
  int rc = ::send(_fd, buf, len, 0);
  if (rc == -1) {
    if (errno == EINTR || (errno == EAGAIN) || errno == EWOULDBLOCK) {
      return rc;
    } else {
      throw SocketException("socket send error", errno, ZONE);
    }
  }
  return rc;
}

/**
 * recv的recvfrom是可以替换使用的，只是recvfrom多了两个参数，可以用来接收对端的地址信息，这个对于udp这种无连接的，可以很方便地进行回复。
 * 而换过来如果你在udp当中也使用recv，那么就不知道该回复给谁了，如果你不需要回复的话，也是可以使用的。
 * 另外就是对于tcp是已经知道对端的，就没必要每次接收还多收一个地址，没有意义，要取地址信息，在accept当中取得就可以加以记录了。
 */
ssize_t Socket::recvfrom(void* buf, size_t len, int flags,
                         struct sockaddr* addr, socklen_t* addr_len) {
  int rc = ::recvfrom(_fd, buf, sizeof(buf), flags, addr, addr_len);
  if (rc == -1) {
    throw SocketException("socket recvfrom error", errno, ZONE);
  }
  return rc;
}

void Socket::close() {
  int rc = ::close(_fd);
  if (rc == -1) {
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

bool isClosed() {
  // struct tcp_info info;
  // int len = sizeof(info);
  // getsockopt(sock, IPPROTO_TCP, TCP_INFO, &info, (socklen_t*)&len);
  // if ((info.tcpi_state == TCP_CLOSE))  // 则断开

  // client 端 close(socket)返回成功，不代表server已经收到所有数据，更不代表
  // 已经server端已经关闭句柄。
  // int ret = getpeername(sockfd, addr, addrlen);
  // if (ret = -1 && error == ENOTCONN)  // 说明连接已经关闭
}