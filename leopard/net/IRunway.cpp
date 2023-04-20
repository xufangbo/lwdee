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
  this->_qps.waitings = [this]() { return this->sockets->size(); };
  this->isET = false;
}

IRunway::~IRunway() {
  if (sockets != nullptr) {
    delete sockets;
    sockets = nullptr;
  }
}

void IRunway::run() {
  this->sockets->start(running);

  while (*running) {
    // this->send();
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

void IRunway::__acceptEvent(epoll_event* evt) {
  // sktlock.lock();
  auto connection = sockets->find(evt->data.fd);
  // sktlock.unlock();
  if (connection == nullptr) {
    logger_error("can't find socket %d", evt->data.fd);
    return;
  }
  if (connection->socket == nullptr) {
    logger_error("socket is null %d", evt->data.fd);
    return;
  }
  if (connection->closed) {
    logger_error("socket has closed %d", evt->data.fd);
    return;
  }
  auto socket = connection->socket;

  if (evt->events & EPOLLIN) {
    this->acceptRecive(socket, evt);
  } else if (evt->events & EPOLLOUT) {
    this->acceptSend(socket);
    // logger_trace("EPOLL OUT do nothing %d", socket->fd());
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

void IRunway::acceptRecive(Socket* socket, epoll_event* evt) {
  int rc = 0;
  do {
    char buf[BUFFER_SIZE] = {0};

    try {
      rc = socket->recv(buf, BUFFER_SIZE, MSG_WAITALL);
      // logger_trace("rc %d", rc);
    } catch (SocketException& ex) {
      ex.trace(ZONE);
      this->close(socket);
      throw ex;
    }

    // leopard_trace("recv - rc:%d,%s", rc, buf);

    if (rc > 0) {
      auto* inputStream = socket->inputStream();
      inputStream->puts(buf, rc);
      if (inputStream->isEnd()) {
        this->_qps.recvs++;
        auto pickedStream = inputStream->pick();
        std::thread t(&IRunway::acceptRequest, this, socket, pickedStream);
        t.detach();
      } else {
        epoll->mod(evt, socket->fd(), isET ? (EPOLLIN | EPOLLET) : (EPOLLIN | EPOLLOUT));
      }
    } else if (rc == -1) {
      // logger_debug("rc == -1");
      epoll->mod(evt, socket->fd(), isET ? (EPOLLIN | EPOLLET) : (EPOLLIN | EPOLLOUT));
    } else if (rc == 0) {
      // leopard_warn("recv closed");
      this->close(socket);
    } else {
      logger_error("rc of recv < -1 ?");
    }
  } while (rc > 0);
}

void IRunway::acceptRequest(Socket* socket, BufferStreamPtr inputStream) {
  try {
    this->__acceptRequest(socket, inputStream);
  } catch (Exception& ex) {
    logger_error("%s %s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
  } catch (std::exception& ex) {
    logger_error("%s", ex.what());
  }
}

ProtocalHeaderPtr IRunway::parseRequest(BufferStream* inputStream) {
  inputStream->moveToHead();

  auto protocal = ProtocalFactory::getProtocal();
  auto header = protocal->getHeader(inputStream);
  auto path = header->path;

  this->_qps.time(header->rec1_sen1());

  return header;
}

void IRunway::acceptSend(Socket* socket) {
  // socket->sendEnabled = true;
}

void IRunway::addSendTask(Socket* socket, BufferStreamPtr outputStream) {
  this->sockets->pushBullet(socket, outputStream);
}

void IRunway::close(Socket* socket) {
  // leopard_warn("close socket %d",socket->fd());

  epoll->del(socket->fd());
  socket->close();

  socket->sendEnabled = false;

  sockets->remove(socket->fd());

  delete socket;
}

void IRunway::join() {
  thread.join();
}

Qps* IRunway::qps() {
  return &_qps;
}
