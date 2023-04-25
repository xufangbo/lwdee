#pragma once

#include <string.h>
#include <sys/epoll.h>
#include <iostream>

/**
 * @brief
 * Epoll有两种模式：ET和LT
 * https://cloud.tencent.com/developer/article/1636224

LT(level triggered，水平触发模式)， epoll的默认方式，一个事件只要有，就会一直触发。同时支持 block 和 non-block socket。
   在这种做法中，内核告诉你一个文件描述符是否就绪了，然后你可以对这个就绪的fd进行IO操作。
   如果你不作任何操作，内核还是会继续通知你的，所以，这种模式编程出错误可能性要小一点。

ET(edge-triggered， 边缘触发模式)，只有一个事件从无到有才会触发。   只支持no-block socket。在这种模式下，
   当描述符从未就绪变为就绪时，内核通过epoll告诉你。然后它会假设你知道文件描述符已经就绪，
   并且不会再为那个文件描述符发送更多的就绪通知，等到下次有新的数据进来的时候才会再次出发就绪事件。
 *
    EPOLLIN   ：表示对应的文件描述符可以读（SOCKET正常关闭）
    EPOLLOUT  ：表示对应的文件描述符可以写

    EPOLLPRI  ：表示对应的文件描述符有紧急的数据可读（表示有带外数据到来）

    EPOLLERR  ：表示对应的文件描述符发生错误(默认注册)

    EPOLLRDHUP：表示读关闭。可能不是所有的内核版本都支持。1、对端发送 FIN (对端调用close 或者 shutdown(SHUT_WR)).2、本端调用 shutdown(SHUT_RD). 当然，关闭 SHUT_RD 的场景很少
    EPOLLHUP  ：表示对应的文件描述符被挂断(默认注册)，表示读写都关闭。

    EPOLLET   ： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的
    EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里*
 *
 */
class Epoll {
 private:
  int fd_epoll;
  int max_event_size;
  epoll_event* _events;

 public:
  int maxSize() { return max_event_size; }

 public:
  Epoll(int maxSize);
  ~Epoll();

  epoll_event& events(int i) { return _events[i]; }

  
  void add(int fd, uint32_t events, void *ptr);
  void mod(int fd,struct epoll_event* event,  uint32_t events, void *ptr);
  void del(int fd);

  int wait(int timeout);
};