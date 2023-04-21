#include "Connection.hpp"

#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "log.hpp"

Connection::Connection(Socket* socket)
    : socket(socket) {
}
Connection::Connection(Socket* socket, BufferStream* outputStream)
    : socket(socket) {
  this->push(outputStream);
}

Connection::~Connection() {
  if (socket != nullptr) {
    delete socket;
    socket = nullptr;
  }

  while (!this->bullets.empty()) {
    auto bullet = this->bullets.front();
    this->bullets.pop();
    delete bullet;
  }
}

void Connection::push(BufferStream* outputStream) {
  auto bullet = new Bullet(outputStream);
  bullets.push(bullet);
}

bool Connection::send() {
  while (!this->bullets.empty()) {
    if (this->closed) {
      return false;
    }
    auto bullet = this->bullets.front();
    if (!bullet->send(this->socket)) {
      return false;
    }
    this->bullets.pop();
    delete bullet;
  }

  _finished = true;
  return true;
}
