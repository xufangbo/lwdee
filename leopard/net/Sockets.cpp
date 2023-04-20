#include "Sockets.hpp"

#include <thread>

#include "log.hpp"

SendTask* Sockets::insert(Socket* s) {
  auto task = std::make_shared<SendTask>(s);
  sockets[s->fd()] = task;
  return task.get();
}

SendTask* Sockets::find(int fd) {
  auto it = sockets.find(fd);
  if (it == sockets.end()) {
    return nullptr;
  } else {
    return it->second.get();
  }
}
void Sockets::remove(int fd) {
  //  mut.lock();
  auto it = sockets.find(fd);
  if (it != sockets.end()) {
    sockets.erase(fd);
  }
  // mut.unlock();
}

void Sockets::pushBullet(Socket* socket, BufferStreamPtr outputStream) {
  auto task = this->find(socket->fd());
  if (task == nullptr) {
    task = this->insert(socket);
  }
  task->push(outputStream);
}

void Sockets::start(bool* running) {

  this->running = running;

  std::thread t(&Sockets::run, this);
  t.detach();
}

void Sockets::run() {
  while (*running) {
    try {
      this->__run();
    } catch (std::exception& ex) {
      logger_error("%s", ex.what());
    }
  }
}

void Sockets::__run() {
  // std::lock_guard lock(sktlock);
  for (auto it : sockets) {
    auto sender = it.second;
    if (sender != nullptr) {  // && sender->socket->sendEnabled
      sender->send();
    }
  }
}