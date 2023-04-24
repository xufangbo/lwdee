#include "IRunway.hpp"

#include <memory>
#include <thread>
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "net/IProtocal.hpp"
#include "net/ProtocalFactory.hpp"
#include "net/log.hpp"

IRunway::IRunway(int id, bool* running)
    : _qps(id), running(running) {
  this->epoll = std::make_shared<Epoll>(1800);
  this->isET = false;
  this->isEOUT = true;
  this->nonBlocking = true;
}

IRunway::~IRunway() {
  // if (connections != nullptr) {
  //   delete connections;
  //   connections = nullptr;
  // }
}

void IRunway::run() {
  this->gererateEnvents();

  while (*running) {
    int waits = epoll->wait(1000);  // ms
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
  // logger_debug("__acceptEvent,fd:%d", evt->data.fd);

  Connection* connection = (Connection*)evt->data.ptr;
  connection->lastTime = Stopwatch::currentMilliSeconds();

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
  int sum = 0;
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
      sum += rc;
      auto inputStream = socket->inputStream();
      inputStream->puts(buf, rc);

      while (inputStream->isEnd()) {
        this->_qps.recvs++;
        auto pickedStream = inputStream->pick();
        // leopard_debug("picked,seq: %d,size: %d", _qps.recvs.load(),pickedStream->size());
        std::thread t(&IRunway::acceptRequest, this, connection, pickedStream);
        t.detach();
      }

      epoll->mod(socket->fd(), evt, EVENTS_IN, connection);
    } else if (rc == -1) {
      if (errno == ECONNRESET) {
        this->close(connection);  //(104)Connection reset by peer
      } else {
        epoll->mod(socket->fd(), evt, EVENTS_IN, connection);
      }
    } else if (rc == 0) {
      // printf("recv closed %d", socket->fd());
      this->close(connection);
    } else {
      logger_error("rc of recv < -1 ?");
    }
  } while (rc > 0);

  // printf("\n> -------------- %d\n", sum);
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

void IRunway::acceptSend(Connection* connection) {
  connection->writable(true);
  connection->send(SendSource::epoll_out);
}

// void IRunway::send(Connection* connection, BufferStream* outputStream) {
//   connection->push(outputStream);
//   connection->send(SendSource::request);
// }

void IRunway::close(Connection* connection) {
  // leopard_warn("close socket %d",socket->fd());

  auto socket = connection->socket;

  epoll->del(socket->fd());

  socket->close();

  // connections->remove(socket->fd());
}

void IRunway::gererateEnvents() {
  EVENTS_NEW = EPOLLIN | EPOLLRDHUP | EPOLLHUP;
  EVENTS_IN = EPOLLIN;
  EVENTS_OUT = EPOLLIN;

  if (isEOUT) {
    EVENTS_NEW |= EPOLLOUT;
    EVENTS_IN |= EPOLLOUT;
    EVENTS_OUT |= EPOLLOUT;
  }
  if (isET) {
    EVENTS_NEW |= EPOLLET;
    EVENTS_IN |= EPOLLET;
    EVENTS_OUT |= EPOLLET;
  }
}

void IRunway::join() {
  thread.join();
}

Qps* IRunway::qps() {
  return &_qps;
}
