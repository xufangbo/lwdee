#include "Connection.hpp"

#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "log.hpp"

Connection::Connection(Socket* socket, Qps* qps)
    : socket(socket), qps(qps) {}

Connection::~Connection() {
  if (socket != nullptr) {
    logger_debug("dispose %d", socket->fd());
    delete socket;
    socket = nullptr;
  }

  while (!this->bullets.empty()) {
    auto bullet = this->bullets.front();
    this->bullets.pop();
    delete bullet;
  }
}

void Connection::close(CloseType closeType){
  if(this->closed){
    return;
  }
  this->closed = true;
  socket->close(closeType);
}

void Connection::push(BufferStream* outputStream) {
  auto bullet = new Bullet(outputStream, qps);
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
