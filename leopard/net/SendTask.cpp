#include "SendTask.hpp"

#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/log.hpp"

bool Bullet::send(Socket* socket) {
  int rc = socket->send(buffer(), leftover());

  if (rc == leftover()) {
    _finished = true;
    socket->onSend();

    auto elapsed = Stopwatch::elapsed(this->start);
    leopard_trace("send finished,elapsed: %.3fs", elapsed);
  } else if (rc == -1) {
    if (errno == EINTR || (errno == EAGAIN) || errno == EWOULDBLOCK) {
      // do nothing
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

bool SendTask::send() {
  if (!this->bullets.empty()) {
    auto bullet = this->bullets.front();
    bool sended = bullet->send(this->socket);
    if (!sended) {
      return false;
    }
    this->bullets.pop_front();
  }

  _finished = true;
  return true;
}
