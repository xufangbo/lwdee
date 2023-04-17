#include "Lane.hpp"

#include <signal.h>
#include <thread>
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/LeopardConfig.hpp"

Lane::Lane(int id, bool* running, SendTaskQueue* sendQueue)
    : IRunway(id, running, sendQueue) {
}

void Lane::start() {
  std::thread(&Lane::run, this).detach();
}

ProtocalHeaderPtr Lane::doHandle(Socket* socket) {
  auto header = IRunway::doHandle(socket);

  auto* inputStream = socket->inputStream();

  auto callback = TcpRequest::find(header->path);
  if (callback != nullptr) {
    (*callback)(inputStream);
    return header;
  }

#ifdef LEOPARD_SUSPEND
  auto suspend = TcpRequest::findSuspend(path);
  if (suspend != nullptr) {
    suspend->callback(inputStream);
    return;
  }
#endif
  logger_error("can't hint path %s", header->path.c_str());

  return header;
}

Socket* Lane::create(const char* ip, int port) {
  Stopwatch sw;

  Socket* socket = new Socket(&_qps);

  socket->connect(ip, port);
  socket->setNonBlocking();
  socket->reusePort();
  socket->setSendBuf(1048576);
  // logger_debug("default sendbufer %d", socket->getSendBuf());    // 425984
  // logger_debug("default revibufer %d", socket->getReciveBuf());  // 131072
  sockets.insert(socket);

  auto eclapse = sw.stop();
  if (eclapse > 1) {
    logger_warn("long time to connect: %lfs", eclapse);
  }

  uint32_t events = EPOLLIN;
  events |= EPOLLOUT;
  events |= (EPOLLRDHUP | EPOLLHUP);

  epoll->add(socket->fd(), isET ? (events | EPOLLET) : events);

  return socket;
}

void Lane::send(Socket* socket, BufferStreamPtr outputStream) {
  sendQueue->push(socket, outputStream);
}

bool Lane::contains(int fd) {
  Socket* socket = sockets.find(fd);
  return socket != nullptr;
}