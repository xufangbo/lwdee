#include "Connection.hpp"

#include "IProtocal.hpp"
#include "ProtocalFactory.hpp"
#include "Runway.hpp"
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "log.hpp"

Connection::Connection(Runway* runway)
    : runway(runway) {
  inStream = ProtocalFactory::createStream();
}

Connection::Connection(int fd, Runway* runway)
    : Connection(runway) {
  this->socket = new Socket(fd, runway->qps());
  socket->setNonBlocking();
}

Connection::~Connection() {
  if (socket != nullptr) {
    // logger_debug("dispose %d", socket->fd());
    delete socket;
    socket = nullptr;
  }

  if (inStream != nullptr) {
    delete inStream;
    inStream = nullptr;
  }

  while (!this->bullets.empty()) {
    auto bullet = this->bullets.front();
    this->bullets.pop();
    delete bullet;
  }
}

int Connection::fd() {
  return socket->fd();
}

BufferStream* Connection::inputStream() {
  return inStream;
}

void Connection::acceptRecive(epoll_event* evt) {
  int rc = 0;
  int sum = 0;
  do {
    try {
      rc = socket->recv(runway->buf, BUFFER_SIZE, MSG_WAITALL);
      // printf("rc-%d ", rc);
    } catch (SocketException& ex) {
      ex.trace(ZONE);
      this->close(CloseType::unknown);
      throw ex;
    }

    // leopard_trace("recv - rc:%d,%s", rc, runway->buf);

    if (rc > 0) {
      sum += rc;
      auto inputStream = inStream;
      inputStream->puts(runway->buf, rc);

      while (inputStream->isEnd()) {
        runway->_qps.recvs++;
        auto pickedStream = inputStream->pick();
        // leopard_debug("picked,seq: %d,size: %d",
        // _qps.recvs.load(),pickedStream->size());
        std::thread t(&Connection::acceptRequest, this, pickedStream);
        t.detach();
      }

      runway->epoll_mod_in(socket->fd(), evt, this);
    } else if (rc == -1) {
      if (errno == ECONNRESET) {
        this->close(CloseType::err_recv);  //(104)Connection reset by peer
      } else {
        runway->epoll_mod_in(socket->fd(), evt, this);
      }
    } else if (rc == 0) {
      // printf("recv closed %d", socket->fd());
      this->close(CloseType::normal);
    } else {
      logger_error("rc of recv < -1 ?");
    }
  } while (rc > 0);

  // printf("\n> -------------- %d\n", sum);
}

void Connection::acceptRequest(BufferStream* inputStream) {
  try {
    this->__acceptRequest(inputStream);
  } catch (Exception& ex) {
    logger_error("%s %s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
  } catch (std::exception& ex) {
    logger_error("%s,%s:%d", ex.what(), __FILE__, __LINE__);
  }
}

void Connection::__acceptRequest(BufferStream* inputStream) {
  auto protocal = ProtocalFactory::getProtocal();
  if (runway->appType == ApplicationType::server) {
    protocal->saccept(this, inputStream);
  } else {
    protocal->caccept(this, inputStream);
  }
}

void Connection::close(CloseType closeType) {
  // 要有地方去释放Connection对象
  // if (this->closed) {
  //   return;
  // }
  this->closed = true;
  runway->epoll_del(socket->fd());
  socket->close(closeType);

  delete this;
}

void Connection::push(BufferStream* outputStream) {
  auto bullet = new Bullet(outputStream, runway->qps());
  bullets.push(bullet);
}

bool Connection::send(SendSource source) {
  // logger_debug("< send wr: %d,source:%d", wr, source);

  std::lock_guard lock(mut);

  if (!wr) {
    return false;
  }

  while (!this->bullets.empty()) {
    if (this->closed) {
      return false;
    }
    auto bullet = this->bullets.front();
    wr = bullet->send(this->socket);
    if (!wr) {
      return false;
    }
    this->bullets.pop();
    delete bullet;
  }
  return true;
}

bool Connection::finished() {
  return bullets.empty();
}

void Connection::writable(bool wr) {
  this->wr = wr;
}
bool Connection::writable() {
  return this->wr;
}
