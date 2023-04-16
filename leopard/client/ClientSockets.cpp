#include "ClientSockets.hpp"

void ClientSockets::insert(Socket* s) {
  // mut.lock();
  sockets->push_back(s);
  // mut.unlock();
}
Socket* ClientSockets::find(int fd) {
  //  mut.lock();
  auto it = std::find_if(sockets->begin(), sockets->end(), [&fd](Socket* i) { return i->fd() == fd; });
  if (it != sockets->end()) {
    return *it;
  }
  // mut.unlock();
  return nullptr;
}
void ClientSockets::remove(Socket* s) {
  //  mut.lock();
  auto it = std::find_if(sockets->begin(), sockets->end(), [s](Socket* i) { return i == s; });
  if (it != sockets->end()) {
    sockets->erase(it);
  }
  // mut.unlock();
}