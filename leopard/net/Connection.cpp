#include "Connection.hpp"

#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/log.hpp"

Connection::Connection(Socket* socket)
    : socket(socket) {
}
Connection::Connection(Socket* socket, BufferStreamPtr outputStream)
    : socket(socket) {
  auto bullet = std::make_shared<Bullet>(outputStream);
  bullets.push_back(bullet);
}

void Connection::push(BufferStreamPtr outputStream) {
  auto bullet = std::make_shared<Bullet>(outputStream);
  bullets.push_back(bullet);
}

bool Connection::send() {
  while (!this->bullets.empty()) {
    auto bullet = this->bullets.front();
    if (!bullet->send(this->socket)) {
      return false;
    }
    this->bullets.pop_front();
  }

  _finished = true;
  return true;
}
