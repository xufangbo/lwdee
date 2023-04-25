#include "Epoll.hpp"
#include "core/Exception.hpp"

Epoll::Epoll(int maxSize)
    : max_event_size(maxSize), _events(new epoll_event[maxSize]) {
  fd_epoll = epoll_create(maxSize);
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

void Epoll::add(int fd, uint32_t events, void* ptr) {
  // 创建节点结构体将监听连接句柄
  epoll_event event;
  event.events = events;
  event.data.ptr = ptr;
  epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd, &event);
}

void Epoll::mod(int fd, struct epoll_event* event, uint32_t events, void* ptr) {
  event->data.ptr = ptr;
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