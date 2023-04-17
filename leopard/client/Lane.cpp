#include "Lane.hpp"

#include <signal.h>
#include <thread>
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/LeopardConfig.hpp"
#include "net/LeopardProtocal.hpp"
#include "net/ProtocalFactory.hpp"

#define BUFFER_SIZE 1024

Lane::Lane(int id,bool *running, SendTaskQueue* sendQueue)
    : _qps(id),running(running), sendQueue(sendQueue) {
  this->epoll =std::make_shared<Epoll>(18000);
  this->_qps.waitings = [this]() { return this->sockets.size(); };
}

void Lane::start() {
  std::thread(&Lane::run,this).detach();
}

void Lane::stop() {
  running = false;
}

void Lane::run() {
  // https://blog.csdn.net/weixin_33196646/article/details/116730365
  signal(SIGPIPE, SIG_IGN);

  while (running) {
    int waits = epoll->wait(100);
    for (int i = 0; i < waits; i++) {
      try {
        auto evt = epoll->events(i);
        handleEvent(evt);
      } catch (EpollException& ex) {
        logger_error("%s", ex.getMessage().c_str());
      } catch (SocketException& ex) {
        logger_error("%s", ex.getMessage().c_str());
      } catch (std::exception& ex) {
        logger_error("%s", ex.what());
      }
    }
  }
}

void Lane::handleEvent(epoll_event& evt) {
  Socket* socket = sockets.find(evt.data.fd);
  if (socket == nullptr) {
    logger_debug("no hint socket %d", evt.data.fd);
    return;
  }

  if (evt.events & EPOLLIN) {
#ifdef LEOPARD_TRACING
    logger_trace("EPOLL IN do nothing");
#endif
    recv(socket, &evt);
  } else if (evt.events & EPOLLOUT) {
#ifdef LEOPARD_TRACING
    logger_trace("EPOLL OUT do nothing");
#endif
  } else if (evt.events & EPOLLHUP) {
    logger_info("close client: EPOLLHUP %d", socket->fd());
    close(socket);
  } else if (evt.events & EPOLLRDHUP) {
    logger_info("close client: EPOLLRDHUP %d", socket->fd());
    close(socket);
  } else if (evt.events & EPOLLERR) {
    logger_debug("close client : EPOLLERR %d", evt.data.fd);
    close(socket);
  } else {
    logger_warn("unkonw epoll events : %d", evt.events);
  }
}

void Lane::recv(Socket* socket, epoll_event* evt) {
  char buf[BUFFER_SIZE] = {0};
  int rc = socket->recv(buf, BUFFER_SIZE, MSG_WAITALL);
#ifdef LEOPARD_TRACING
  // logger_trace("recv - %s,rc:%d", buf, rc);
#endif

  auto* inputStream = socket->inputStream();

  inputStream->puts(buf, rc);

  if (rc == 0) {
#ifdef LEOPARD_TRACING
    logger_info("client socket(%d) closed on rc == 0", socket->fd());
#endif
    close(socket);
    return;
  }

  if (inputStream->isEnd()) {
    this->_qps.inputs++;

    handleRequest(socket);
    inputStream->clean();
    close(socket);

  } else {
    epoll->mod(evt, socket->fd(), isET ? (EPOLLIN | EPOLLET) : EPOLLIN);
  }
}

void Lane::handleRequest(Socket* socket) {
  auto protocal = ProtocalFactory::getProtocal();

  auto* inputStream = socket->inputStream();
  inputStream->reset();

  auto header = protocal->getHeader(inputStream);
  auto path = header->path;

  this->_qps.time(header->elapsed);

#ifdef LEOPARD_TRACING
  auto time = Stopwatch::currentMilliSeconds() - header->time;
  if (time > 1000) {
    logger_trace("> recive %s , too long %lfs", path.c_str(), time * 1.0 / 1000);
  } else {
    logger_trace("> recive %s", path.c_str());
  }
#endif

  auto callback = TcpRequest::find(path);
  if (callback != nullptr) {
    (*callback)(inputStream);
    return;
  }

#ifdef LEOPARD_SUSPEND
  auto suspend = TcpRequest::findSuspend(path);
  if (suspend != nullptr) {
    suspend->callback(inputStream);
    return;
  }
#endif
  logger_error("can't hint path %s", path.c_str());
}

void Lane::close(Socket* socket) {
  epoll->del(socket->fd());
  socket->close();

  sockets.remove(socket);
  delete socket;
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