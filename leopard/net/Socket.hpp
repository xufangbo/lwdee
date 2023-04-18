#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "BufferStream.hpp"
#include "Qps.hpp"

class Socket {
 protected:
  int _fd;
  BufferStreamPtr _inputStream;
  Qps *qps = nullptr;

 public:
  Socket( Qps* qps);

  Socket(int fd,Qps *qps);

  /**
   * @brief 数据发送完成事件
   * 主要是更新qps.outputs
   */
  void onSend();

  int& fd() { return _fd; }
  BufferStream *inputStream() { return _inputStream.get(); }
  // BufferStream *outputStream() { return _outputStream.get(); }

  void bind(const char* ip, int port);

  void listen();

  int accept();

  void connect(const char* ip, int port);

  /**
   * read是阻塞读数据
   * 数据在不超过指定的长度的时候有多少读多少，没有数据则会一直等待。
   * 所以一般情况下：我们读取数据都需要采用循环读的方式读取数据，因为一次read 完毕不能保证读到我们需要长度的数据，read 完一次需要判断读到的数据长度再决定是否还需要再次读取。
   * 会发生返回值比指定长度短的情况
   *
   * > 0 : 表示实际所读的字节数
   * = 0 : 表示已经读到文件的结束了
   * < 0 : 表示出现了错误.如果错误为EINTR说明读是由中断引起的, 如果是ECONNREST表示网络连接出了问题.
   */
  ssize_t read(void* buf, size_t len);

  /**
   * 在进行TCP协议传输的时候，要注意数据流传输的特点，recv和send不一定是一一对应的（一般情况下是一一对应），
   * 也就是说并不是send一次，就一定recv一次就接收完，有可能send一次，recv多次才接收完，也可能send多次，一次recv就接收完了。
   * TCP协议会保证数据的有序完整的传输，但是如何去正确完整的处理每一条信息，是程序员的事情。
   *
   * 例如：服务器在循环recv，recv的缓冲区大小为100byte，客户端在循环send，
   * 每次send 6byte数据，则recv每次收到的数据可能为6byte，12byte，18byte，这是随机的，编程的时候注意正确的处理。
   */
  ssize_t write(void* buf, size_t len);

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
   * return flags
   * 0            : recv和read一样
   * MSG_DONTROUTE: 这个标志告诉IP.目的主机在本地网络上面,没有必要查找表.这个标志一般用网络诊断和路由程序里面.
   * MSG_OOB      : 接受或者发送带外数据
   * MSG_PEEK     : 是recv函数的使用标志,表示只是从系统缓冲区中读取内容,而不清除系统缓冲区的内容.这样下次读的时候,仍然是一样的内容.一般在有多个进程读写数据时可以使用这个标志.
   * MSG_WAITALL  : recv函数的使用标志,表示等到所有的信息到达时才返回.使用这个标志的时候recv回一直阻塞,直到指定的条件满足,或者是发生了错误.
   *                     1)当读到了指定的字节时,函数正常返回.返回值等于len
   *                     2)当读到了文件的结尾时,函数正常返回.返回值小于len
   *                     3)当操作发生错误时,返回-1,且设置错误为相应的错误号(errno)
   *
   * MSG_NOSIGNAL : is a flag used by send a SIGPIPE signal on errors on stream oriented sockets when the other end breaks the connection.
   *                The EPIPE error is still returned as normal。
   *                Though it is in some Berkely sockets APIs (notably Linux) it does not exist in what some refer to as the reference implementation,
   *                FreeBSD, which instead uses a socket option.
   * SO_NOSIGPIPE : 对于服务器端，我们可以使用这个标志。目的是不让其发送SIG_PIPE信号，导致程序退出。
   */
  ssize_t recv(void* buf, size_t len, int flags);

  ssize_t send(void* buf, size_t len);

  /**
   * recv的recvfrom是可以替换使用的，只是recvfrom多了两个参数，可以用来接收对端的地址信息，这个对于udp这种无连接的，可以很方便地进行回复。
   * 而换过来如果你在udp当中也使用recv，那么就不知道该回复给谁了，如果你不需要回复的话，也是可以使用的。
   * 另外就是对于tcp是已经知道对端的，就没必要每次接收还多收一个地址，没有意义，要取地址信息，在accept当中取得就可以加以记录了。
   */
  ssize_t recvfrom(void* buf, size_t len, int flags, struct sockaddr* addr, socklen_t* addr_len);

  void close();
  void shutdown(int how = SHUT_RDWR);

  int getError();
  /**
   * @brief 设置句柄非阻塞
   */
  void setNonBlocking();
  void reusePort();
  void setSendBuf(int size);
  void setReciveBuf(int size);
  int getSendBuf();
  int getReciveBuf();
};