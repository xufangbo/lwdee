#include "SocketWaiter.hpp"

#include "log.hpp"

void SocketWaiter_t::notify(WaitStatus status) {
  this->status = status;
}

float SocketWaiter_t::wait(float timeout) {
  while (sw.elapsed() < timeout) {
    if (status == WaitStatus::succeed) {
      return sw.stop();
    } else if (status == WaitStatus::timeout) {
      sw.stop();
      throw Exception("timeout", ZONE);
    } else if (status == WaitStatus::nohint) {
      return sw.stop();
      throw Exception("nohint", ZONE);
    } else {
      usleep(1000 * 100);
    }
  }
  leopard_error("[%d] timeout in %lfs", id, timeout);
  return timeout;
}


/**
 * 1. 客户端sends数不够
 * 1. 服务端sockets数不够，客户端sends数不够，不知道有没有因果关系
 * 1. 服务端sockets只有9个，但是客户端也不是发了900个，而是500个
 * 1. 服务端sockets只有8个，客户单sends只有800
 * 1. 客户端recive是1000，发送怎么是983呢？但是bullets是1000
 * 1. 客户端csv中的recive是1000，但是日志中recive是996？
 * 1. 接收了999个，但是超时的几十个
*/
