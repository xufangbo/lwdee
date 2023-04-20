#include "Lane.hpp"

#include <signal.h>
#include <thread>
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/LeopardConfig.hpp"
#include "net/log.hpp"

Lane::Lane(int id, bool* running)
    : IRunway(id, running) {
}

void Lane::start() {
  thread = std::thread(&Lane::run, this);
}

void Lane::acceptEvent(epoll_event* evt) {
  IRunway::__acceptEvent(evt);
}

void Lane::__acceptRequest(Connection* connection, BufferStreamPtr inputStream) {
  ClientSocket* clientSocket = (ClientSocket*)(connection->socket);

  auto header = this->parseRequest(inputStream.get());
  header->rec2 = Stopwatch::currentMilliSeconds() - header->sen1;

  leopard_trace(header->to_string().c_str());

  auto callback = TcpRequest::find(header->path);

  WaitStatus waitStatus = WaitStatus::waiting;
  if (callback != nullptr) {
    (*callback)(inputStream.get());
    waitStatus = WaitStatus::succeed;
  } else {
    waitStatus = WaitStatus::nohint;
    logger_error("can't hint path %s", header->path.c_str());
  }

  auto waiter = clientSocket->popWaiter();
  if (waiter == nullptr || waiter.use_count() == 0) {
    logger_error("waiter is null");
  } else {
    waiter->notify(waitStatus);
  }

#ifdef LEOPARD_SUSPEND
  auto suspend = TcpRequest::findSuspend(path);
  if (suspend != nullptr) {
    suspend->callback(inputStream);
    return;
  }
#endif
}

Connection* Lane::create(const char* ip, int port) {
  Stopwatch sw;

  ClientSocket* socket = new ClientSocket(this, &_qps);
  // leopard_warn("new client socket fd : %d", connection->socket->fd());

  socket->connect(ip, port);
  socket->setNonBlocking();
  // socket->reusePort();
  // socket->setSendBuf(1048576);
  // logger_debug("default sendbufer %d", socket->getSendBuf());    // 425984
  // logger_debug("default revibufer %d", socket->getReciveBuf());  // 131072
  auto connection = connections->insert(socket);

  auto eclapse = sw.stop();
  if (eclapse > 1) {
    leopard_warn("long time to connect: %lfs", eclapse);
  }

  uint32_t events = EPOLLIN;
  events |= EPOLLOUT;
  events |= (EPOLLRDHUP | EPOLLHUP);

  epoll->add(socket->fd(), isET ? (events | EPOLLET) : events);

  return connection;
}

void Lane::send(Connection* connection, BufferStreamPtr outputStream) {
  // sendQueue->push(socket, outputStream);
  this->addSendTask(connection, outputStream);
}

// void Lane::close(int fd) {
//   auto it = connections.find(fd);
//   if (it == connections.end()) {
//     logger_error("can't find fd %d", fd);
//     return;
//   }

//   if (it->second->socket == nullptr) {
//     logger_error("socket is null %d", fd);
//   }

//   if (it->second->closed) {
//     logger_error("socket has closed %d", fd);
//   }

//   IRunway::close(it->second->socket);
// }

// bool Lane::contains(int fd) {
//   Connection* connection = connections.find(fd);
//   return socket != nullptr;
// }