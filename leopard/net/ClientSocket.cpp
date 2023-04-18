#include "ClientSocket.hpp"

void ClientSocket::pushWaiter(SocketWaiter waiter) {
  waiters.push(waiter);
  // printf("< put waiter %d / %d\n", waiters.size(), _fd);
}

SocketWaiter ClientSocket::popWaiter() {
  if (waiters.empty()) {
    // printf("pop waiter error %d \n", _fd);
    return nullptr;
  }
  SocketWaiter waiter = waiters.front();
  waiters.pop();
  // printf("> pop waiter %d / %d\n", waiters.size(), _fd);
  return waiter;
}

void ClientSocket::wait() {
  while (!waiters.empty()) {
    auto waiter = waiters.front();
    waiter->wait();
    waiters.pop();
  }
}