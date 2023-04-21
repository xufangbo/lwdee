#include "IRunway.hpp"

#include <thread>
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "net/LeopardProtocal.hpp"
#include "net/ProtocalFactory.hpp"
#include "net/log.hpp"

IRunway::IRunway(int id, bool* running)
    : _qps(id), running(running) {
  this->epoll = std::make_shared<Epoll>(1800);
  this->_qps.waitings = [this]() { return this->connections->size(); };
  this->isET = false;
  this->isEOUT = false;
}

IRunway::~IRunway() {
  if (connections != nullptr) {
    delete connections;
    connections = nullptr;
  }
}

void IRunway::run() {
  this->connections->start(running);

  while (*running) {
    int waits = epoll->wait(1000);
    for (int i = 0; i < waits; i++) {
      try {
        auto evt = epoll->events(i);
        this->acceptEvent(&evt);
      } catch (Exception& ex) {
        logger_error("%s %s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
      } catch (std::exception& ex) {
        logger_error("%s,%s:%d", ex.what(), __FILE__, __LINE__);
      }
    }
  }
}

void IRunway::__acceptEvent(epoll_event* evt) {
  // logger_debug("__acceptEvent");
  auto connection = connections->find(evt->data.fd);
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
    this->acceptRecive(connection, evt);
  } else if (evt->events & EPOLLOUT) {
    this->acceptSend(connection);
    // logger_trace("EPOLL OUT  %d", connection->socket->fd());
  } else if (evt->events & EPOLLHUP) {
    leopard_info("close client: EPOLLHUP %d", connection->socket->fd());
    this->close(connection);
  } else if (evt->events & EPOLLRDHUP) {
    leopard_info("close client: EPOLLRDHUP %d", connection->socket->fd());
    this->close(connection);
  } else if (evt->events & EPOLLERR) {
    leopard_info("close client : EPOLLERR %d", connection->socket->fd());
    this->close(connection);
  } else {
    leopard_warn("unkonw epoll events : %d", evt->events);
  }
}

void IRunway::acceptRecive(Connection* connection, epoll_event* evt) {
  // printf("< --------------\n");
  // logger_trace("--------------");
  int rc = 0;

  do {
    auto socket = connection->socket;
    try {
      rc = socket->recv(buf, BUFFER_SIZE, MSG_WAITALL);
      // printf("rc-%d ", rc);
    } catch (SocketException& ex) {
      ex.trace(ZONE);
      this->close(connection);
      throw ex;
    }

    // leopard_trace("recv - rc:%d,%s", rc, buf);

    if (rc > 0) {
      auto inputStream = socket->inputStream();
      inputStream->puts(buf, rc);
      if (inputStream->isEnd()) {
        this->_qps.recvs++;
        auto pickedStream = inputStream->pick();
        std::thread t(&IRunway::acceptRequest, this, connection, pickedStream);
        t.detach();
      } else {
        // epoll->mod(evt, socket->fd(), gererateEnvents());
      }
    } else if (rc == -1) {
      // logger_debug("rc == -1");
      // epoll->mod(evt, socket->fd(), gererateEnvents());
    } else if (rc == 0) {
      // printf("recv closed %d", socket->fd());
      this->close(connection);
    } else {
      logger_error("rc of recv < -1 ?");
    }
  } while (rc > 0);

  // printf("\n> --------------\n");
}

void IRunway::acceptRequest(Connection* connection, BufferStream* inputStream) {
  try {
    this->__acceptRequest(connection, inputStream);
  } catch (Exception& ex) {
    logger_error("%s %s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
  } catch (std::exception& ex) {
    logger_error("%s,%s:%d", ex.what(), __FILE__, __LINE__);
  }
}

void IRunway::parseRequest(BufferStream* inputStream, ProtocalHeader* header) {
  inputStream->moveToHead();

  auto protocal = ProtocalFactory::getProtocal();
  protocal->parseHeader(inputStream, header);
  auto path = header->path;

  this->_qps.time(header->rec1_sen1());
}

void IRunway::acceptSend(Connection* connection) {
  // connection->socket->sendEnabled = true;
}

void IRunway::addSendTask(Connection* connection, BufferStream* outputStream) {
  this->connections->pushBullet(connection, outputStream);
}

void IRunway::close(Connection* connection) {
  // leopard_warn("close socket %d",socket->fd());

  auto socket = connection->socket;

  epoll->del(socket->fd());

  socket->close();

  connections->remove(socket->fd());
}

inline uint32_t IRunway::gererateEnvents() {
  uint32_t events = EPOLLIN;
  if (isEOUT) {
    events |= EPOLLOUT;
  }
  if (isET) {
    events |= EPOLLET;
  }
  return events;
}

void IRunway::join() {
  thread.join();
}

Qps* IRunway::qps() {
  return &_qps;
}
