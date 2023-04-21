#include "Connections.hpp"

#include <thread>

#include "log.hpp"

// #define lock std::lock_guard lock(mut)
#define lock  //

#define lock_tasks std::lock_guard lock_tasks(mut_task)
// #define lock_tasks  //

Connection* Connections::insert(Socket* s) {
  auto connection = new Connection(s);
  lock;
  items[s->fd()] = connection;
  return connection;
}

Connection* Connections::find(int fd) {
  lock;
  auto it = items.find(fd);
  if (it == items.end()) {
    return nullptr;
  } else {
    return it->second;
  }
}
void Connections::remove(int fd) {
  auto it = items.find(fd);
  if (it != items.end()) {
    auto connection = it->second;
    connection->closed = true;
    // delete socket; 通过Connection析构函数来删除
    // 是否要判断在task中也删除？
    lock;
    items.erase(fd);
  }
}

void Connections::run() {
  while (*running) {
    try {
      this->__run();
    } catch (std::exception& ex) {
      logger_error("%s,%s:%d", ex.what(), __FILE__, __LINE__);
    }
  }
}

void Connections::__run() {
  if (tasks.empty()) {
    usleep(1000);
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

void Connections::pushBullet(Connection* connection, BufferStream* outputStream) {
  connection->push(outputStream);
  lock_tasks;
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
  return items.size();
}
void Connections::clear() {
  items.clear();
}