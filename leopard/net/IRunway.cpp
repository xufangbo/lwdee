#include "IRunway.hpp"

#include <thread>
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "net/LeopardProtocal.hpp"
#include "net/ProtocalFactory.hpp"
#include "net/log.hpp"

#define BUFFER_SIZE 1024

IRunway::IRunway(int id, bool* running, SendTaskQueue* sendQueue)
    : _qps(id), running(running), sendQueue(sendQueue) {
  this->epoll = std::make_shared<Epoll>(1800);
  this->_qps.waitings = [this]() { return this->sockets.size(); };
}

void IRunway::run() {
  while (*running) {
    int waits = epoll->wait(100);
    for (int i = 0; i < waits; i++) {
      try {
        auto evt = epoll->events(i);
        this->acceptEvent(&evt);
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

void IRunway::acceptRecive(epoll_event* evt) {
  auto socket = sockets.find(evt->data.fd);
  if (socket == nullptr) {
    logger_error("no hint socket %d", evt->data.fd);
    return;
  }

  if (evt->events & EPOLLIN) {
    this->doAcceptRecive(socket, evt);
  } else if (evt->events & EPOLLOUT) {
    epollOut = true;
    // logger_trace("EPOLL OUT do nothing");
  } else if (evt->events & EPOLLHUP) {
    leopard_info("close client: EPOLLHUP %d", socket->fd());
    this->close(socket);
  } else if (evt->events & EPOLLRDHUP) {
    leopard_info("close client: EPOLLRDHUP %d", socket->fd());
    this->close(socket);
  } else if (evt->events & EPOLLERR) {
    leopard_info("close client : EPOLLERR %d", socket->fd());
    this->close(socket);
  } else {
    leopard_warn("unkonw epoll events : %d", evt->events);
  }
}

void IRunway::doAcceptRecive(Socket* socket, epoll_event* evt) {
  char buf[BUFFER_SIZE] = {0};
  int rc = 0;
  try {
    rc = socket->recv(buf, BUFFER_SIZE, MSG_WAITALL);
  } catch (SocketException& ex) {
    ex.trace(ZONE);
    this->close(socket);
    throw ex;
  }

  // leopard_trace("recv - %s,rc:%d", buf, rc);

  auto* inputStream = socket->inputStream();
  inputStream->puts(buf, rc);

  if (rc == 0) {
    this->close(socket);
  } else if (inputStream->isEnd()) {
    this->_qps.inputs++;
    std::thread t(&IRunway::acceptRequest, this, socket);
    t.detach();

  } else {
    epoll->mod(evt, socket->fd(), isET ? (EPOLLIN | EPOLLET) : (EPOLLIN));
  }
}

void IRunway::acceptRequest(Socket* socket) {
  try {
    this->doAcceptRequest(socket);
  } catch (Exception& ex) {
    logger_error("%s %s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
  } catch (std::exception& ex) {
    logger_error("%s", ex.what());
  }
}

ProtocalHeaderPtr IRunway::parseRequest(Socket* socket) {
  auto* inputStream = socket->inputStream();
  inputStream->reset();

  auto protocal = ProtocalFactory::getProtocal();
  auto header = protocal->getHeader(inputStream);
  auto path = header->path;

  this->_qps.time(header->elapsed);

  auto time = Stopwatch::currentMilliSeconds() - header->time;
  if (time > 1000) {
    leopard_info("accept %s , too long %lfs", path.c_str(), time * 1.0 / 1000);
  } else {
    leopard_trace("accept %s", path.c_str());
  }

  return header;
}

void IRunway::close(Socket* socket) {
  epoll->del(socket->fd());
  socket->close();

  sockets.remove(socket);
  delete socket;
}

void IRunway::join() {
  thread.join();
}

Qps* IRunway::qps() {
  return &_qps;
}
