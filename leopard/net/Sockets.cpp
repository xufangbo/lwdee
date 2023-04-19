#include "Sockets.hpp"

void Sockets::insert(Socket* s) {
  // mut.lock();
  sockets->push_back(s);
  // mut.unlock();
}
Socket* Sockets::find(int fd) {
  //  mut.lock();
  // auto it = std::find_if(sockets->begin(), sockets->end(), [&fd](Socket* i) { return i->fd() == fd; });
  // if (it != sockets->end()) {
  //   return *it;
  // }

  for (Socket* socket : *sockets) {
    if(socket == nullptr){
      printf("socket is removed?");
      return nullptr;
    }
    if (socket->fd() == fd) {
      return socket;
    }
  }
  // mut.unlock();
  return nullptr;
}
void Sockets::remove(Socket* s) {
  //  mut.lock();
  auto it = std::find_if(sockets->begin(), sockets->end(), [s](Socket* i) { return i == s; });
  if (it != sockets->end()) {
    sockets->erase(it);
  }
  // mut.unlock();
}