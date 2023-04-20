#include "Connections.hpp"

#include <thread>

#include "log.hpp"

// #define lock std::lock_guard lock(mut)
#define lock  //

#define lock_tasks std::lock_guard lock_tasks(mut_task)
#define lock_tasks  //

Connection* Connections::insert(Socket* s) {
  auto task = std::make_shared<Connection>(s);
  lock;
  sockets[s->fd()] = task;
  return task.get();
}

Connection* Connections::find(int fd) {
  lock;
  auto it = sockets.find(fd);
  if (it == sockets.end()) {
    return nullptr;
  } else {
    return it->second.get();
  }
}
void Connections::remove(int fd) {
  auto it = sockets.find(fd);
  if (it != sockets.end()) {
    lock;
    sockets.erase(fd);
  }
}

void Connections::run() {
  while (*running) {
    try {
      this->__run();
    } catch (std::exception& ex) {
      logger_error("%s", ex.what());
    }
  }
}

void Connections::__run() {
  if (tasks.empty()) {
    usleep(1000 * 10);
    // printf("-");
    fflush(stdout);
    return;
  }

  for (auto it = tasks.begin(); it != tasks.end(); it++) {
    Connection* task = *it;
    task->send();
    if (task->finished()) {
      removedTasks.push_back(task);
    }
  }

  this->removeTasks();
}

void Connections::pushBullet(Connection* connection, BufferStreamPtr outputStream) {
  connection->push(outputStream);
  auto it = std::find_if(tasks.begin(), tasks.end(), [&connection](Connection* i) { return i == connection; });
  if (it == tasks.end()) {
    tasks.emplace_back(connection);
  }
}

void Connections::removeTasks() {
  lock_tasks;
  for (Connection* task : removedTasks) {
    tasks.remove(task);
  }

  removedTasks.clear();
}

void Connections::start(bool* running) {
  this->running = running;

  std::thread t(&Connections::run, this);
  t.detach();
}

size_t Connections::size() {
  return sockets.size();
}
void Connections::clear() {
  sockets.clear();
}