#include "Connection.hpp"

#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/log.hpp"

std::atomic<int> Bullet::cout(0);

bool Bullet::send(Socket* socket) {
  int rc = socket->send(buffer(), leftover());

  if (rc == leftover()) {
    _finished = true;
    socket->onSend();

    auto elapsed = Stopwatch::elapsed(this->start);
    Bullet::cout++;
    leopard_info("send finished,elapsed: %.3fs,count: [ %d ]", elapsed, Bullet::cout.load());
  } else if (rc == -1) {
    socket->sendEnabled = false;
    if (errno == EINTR || (errno == EAGAIN) || errno == EWOULDBLOCK) {
      leopard_trace("EAGAIN");
    } else {
      logger_error("socket send error(%d)%s", errno, strerror(errno));
      return false;
    }
  } else {
    leopard_trace("move on to %ld/%ld", outputStream->size(), leftover());
    moveon(rc);
  }
  return true;
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
