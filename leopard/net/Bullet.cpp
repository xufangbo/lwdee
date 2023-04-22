#include "Bullet.hpp"

#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/log.hpp"

std::atomic<int> Bullet::cout(0);

Bullet::~Bullet() {
  if (outputStream != nullptr) {
    delete outputStream;
    outputStream = nullptr;
  }
}

bool Bullet::send(Socket* socket) {
  if (socket == nullptr) {
    throw Exception("sockeet is null ,can't send bullet", ZONE);
  }

  int rc = socket->send(buffer(), leftover());

  if (rc == leftover()) {
    _finished = true;
    socket->onSend();
    Bullet::cout++;
    leopard_trace("        sended,[%d],size:%d,rc=%d", Bullet::cout.load(),outputStream->size(), rc);
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

// bool Bullet::send(Socket* socket) {
//   if (socket == nullptr) {
//     throw Exception("sockeet is null ,can't send bullet", ZONE);
//   }

//   int rc = 0;
//   do {
//     rc = socket->send(buffer(), leftover());

//     if (rc == leftover()) {
//       _finished = true;
//       socket->onSend();

//       auto elapsed = Stopwatch::elapsed(this->start);
//       Bullet::cout++;
//       leopard_trace("        sended,[%d],rc=%d", Bullet::cout.load(), rc);
//       return true;
//     } else if (rc == -1) {
//       if (errno == EINTR || (errno == EAGAIN) || errno == EWOULDBLOCK) {
//         // leopard_trace("EAGAIN");
//         // return false;
//       } else {
//         logger_error("socket send error(%d)%s", errno, strerror(errno));
//         return false;
//       }
//     } else {
//       moveon(rc);
//       leopard_trace("move on to %ld/%ld,rc=%d", outputStream->size(), pos, rc);
//       // return false;
//     }
//   } while (rc > 0);

//   // return true;
// }