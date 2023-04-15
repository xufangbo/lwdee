#include "SocketScheduler.hpp"

#include <signal.h>

#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/LeopardConfig.hpp"
#include "net/LeopardProtocal.hpp"

#define BUFFER_SIZE 1024

bool SocketScheduler::_running = false;
bool SocketScheduler::isET = true;
bool SocketScheduler::tracing = true;
int SocketScheduler::waits = 0;
int SocketScheduler::tps = 0;
int SocketScheduler::unhandles = 0;
std::thread SocketScheduler::tpsThread;
std::thread SocketScheduler::runningThread;
std::shared_ptr<Epoll> SocketScheduler::epoll;
ClientSockets SocketScheduler::clients;

void SocketScheduler::start() {
  if (_running) {
    return;
  } else {
    _running = true;
  }
  epoll = std::make_shared<Epoll>(18000);
  runningThread = std::thread(&SocketScheduler::running);
  tpsThread = std::thread(&SocketScheduler::tpsJob);
}

void SocketScheduler::stop() {
  if (_running) {
    // runningThread.stop();
    // tpsThread.stop();
    // epoll.reset();
  }

  _running = false;
}

void SocketScheduler::running() {
  // https://blog.csdn.net/weixin_33196646/article/details/116730365
  signal(SIGPIPE, SIG_IGN);

  while (_running) {
    // if (!tracing) newConnect();

    waits = epoll->wait(100);
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
    usleep(1000000 / 1000);
  }
}

void SocketScheduler::join() { runningThread.join(); }

void SocketScheduler::handleEvent(epoll_event& evt) {
  Socket* socket = clients.find(evt.data.fd);
  if (socket == nullptr) {
    logger_debug("no hint socket %d", evt.data.fd);
    return;
  }

  if (evt.events & EPOLLIN) {
    recv(socket, &evt);
  } else if (evt.events & EPOLLOUT) {
    if (tracing) logger_trace("EPOLL OUT do nothing");
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

void SocketScheduler::recv(Socket* socket, epoll_event* evt) {
  char buf[BUFFER_SIZE] = {0};
  int rc = socket->recv(buf, BUFFER_SIZE, MSG_WAITALL);
  // if (tracing) logger_trace("recv - %s,rc:%d", buf, rc);

  auto* inputStream = socket->inputStream();

  inputStream->puts(buf, rc);

  if (rc == 0) {
    if (tracing)
      logger_info("client socket(%d) closed on rc == 0", socket->fd());
    close(socket);
    return;
  }

  if (inputStream->isEnd()) {
    unhandles--;

    handleRequest(socket);

    inputStream->clean();
    close(socket);

  } else {
    epoll->mod(evt, socket->fd(), isET ? (EPOLLIN | EPOLLET) : EPOLLIN);
  }
}

void SocketScheduler::handleRequest(Socket* socket) {
  auto* inputStream = socket->inputStream();
  inputStream->reset();
  auto total_len = inputStream->get<uint64_t>();
  auto path_len = inputStream->get<uint32_t>();
  std::string path = inputStream->getString(path_len);

  logger_trace("callback %s", path.c_str());

  auto callback = TcpRequest::find(path);
  if (callback != nullptr) {
    (*callback)(inputStream);
    return;
  }

  auto suspend = TcpRequest::findSuspend(path);
  if (suspend != nullptr) {
    suspend->callback(inputStream);
  } else {
    logger_error("can't hint path %s", path.c_str());
  }
}

void SocketScheduler::close(Socket* socket) {
  epoll->del(socket->fd());
  socket->close();

  clients.remove(socket);

  delete socket;
}

SocketClientPtr SocketScheduler::newClient(const char* ip, int port) {
  Stopwatch sw;

  Socket* socket = new Socket();
  SocketClientPtr client = std::make_shared<SocketClient>(socket);

  socket->connect(ip, port);
  socket->setNonBlocking();
  // socket->reusePort();
  // logger_debug("connect");
  clients.insert(socket);

  auto eclapse = sw.stop();
  if (eclapse > 1) {
    logger_warn("long time to connect: %lfs", eclapse);
  }

  uint32_t events = EPOLLIN;
  events |= EPOLLOUT;
  events |= (EPOLLRDHUP | EPOLLHUP);

  epoll->add(socket->fd(), isET ? (events | EPOLLET) : events);

  tps++;

  return client;
}

void SocketScheduler::send(Socket* socket, void* buffer, size_t len) {
  Stopwatch sw;

  socket->send(buffer, len);

  unhandles++;

  // epoll->mod(&evt, clientfd,EPOLLIN | EPOLLET | EPOLLONESHOT | EPOLLRDHUP |
  // EPOLLHUP);
  auto eclapse = sw.stop();
  if (eclapse > 1) {
    logger_debug("long time to send: %lfs", eclapse);
  }
}

bool SocketScheduler::contains(int fd) {
  Socket* socket = clients.find(fd);
  return socket != nullptr;
}

void SocketScheduler::tpsJob() {
  // for (;;) {
  //   sleep(1);
  //   logger_trace("sockets:%4d,unhandles:%4d,TPS:%4d,epoll wait:%4d",
  //                clients->size(), unhandles, tps, epoll->wait(0));
  //   tps = 0;
  // }
}