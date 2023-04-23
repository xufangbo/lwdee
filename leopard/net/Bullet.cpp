#include "Bullet.hpp"

#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/log.hpp"

Bullet::Bullet(BufferStream* outputStream, Qps* qps)
    : outputStream(outputStream), qps(qps) {
  qps->sends++;
  leopard_trace("push [%d]", qps->sends.load());
}

Bullet::~Bullet() {
  if (outputStream != nullptr) {
    delete outputStream;
    outputStream = nullptr;
  }
}

bool Bullet::send(Socket* socket) {
  int rc = socket->send(buffer(), leftover());

  if (rc == leftover()) {
    _finished = true;
    qps->bullets++;
    leopard_trace("bulleted [%d],size:%d,rc=%d", qps->bullets.load(), outputStream->size(), rc);
    return true;
  } else if (rc == -1) {
    if (errno == EINTR || (errno == EAGAIN) || errno == EWOULDBLOCK) {
      // leopard_trace("EAGAIN");
      return false;
    } else {
      logger_error("socket send error(%d)%s", errno, strerror(errno));
      return false;
    }
  } else {
    moveon(rc);
    // leopard_trace("move on to %ld/%ld,rc=%d", outputStream->size(), pos, rc);
    return false;
  }
}