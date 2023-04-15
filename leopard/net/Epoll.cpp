#include "Epoll.hpp"
#include "core/Exception.hpp"

Epoll::Epoll(int maxSize)
    : max_event_size(maxSize), _events(new epoll_event[maxSize]) {
  fd_epoll = epoll_create(5);
  if (fd_epoll == -1) {
    throw EpollException("epoll create error", errno, ZONE);
  }
}

Epoll::~Epoll() {
  if (_events != nullptr) {
    delete[] _events;
    _events = nullptr;
  }

  // 如何注销epoll对象？
}

/**
 * @brief
 *
 * @param fd
 * @param events  EPOLLIN | EPOLLET | EPOLLRDHUP
 */
void Epoll::add(int fd, uint32_t events) {
  // 创建节点结构体将监听连接句柄
  epoll_event event;
  event.data.fd = fd;
  // 设置该句柄为边缘触发（数据没处理完后续不会再触发事件，水平触发是不管数据有没有触发都返回事件），
  event.events = events;
  // 添加监听连接句柄作为初始节点进入红黑树结构中，该节点后续处理连接的句柄
  epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd, &event);
}

void Epoll::mod(int fd, struct epoll_event* event) {
  epoll_ctl(fd_epoll, EPOLL_CTL_MOD, fd, event);
}

void Epoll::mod(struct epoll_event* event,int fd,uint32_t events) {
  event->data.fd = fd;
  event->events = events;  // 将事件设置为写事件返回数据给客户端
  epoll_ctl(fd_epoll, EPOLL_CTL_MOD, fd, event);
}

void Epoll::del(int fd) {
  epoll_ctl(fd_epoll, EPOLL_CTL_DEL, fd, 0);
}

int Epoll::wait(int timeout) {
  int number = epoll_wait(fd_epoll, _events, maxSize(), timeout);
  if (number < 0 && errno != EINTR) {
    throw EpollException("epoll wait error", errno, ZONE);
  }
  // if (number == maxSize()) {
  //   std::cout << "epoll wait number : " << number << std::endl;
  //   throw EpollException(std::string("epoll out range ") + std::to_string(maxSize()));
  // }
  return number;
}