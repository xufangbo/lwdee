#include "Connections.hpp"

#include <thread>

#include "log.hpp"

#define lock_connections std::lock_guard lock(mut)
#define lock_tasks std::lock_guard lock_tasks(mut_task)

Connection* Connections::insert(Socket* s) {
  auto connection = new Connection(s,qps);
  lock_connections;
  items[s->fd()] = connection;
  return connection;
}

void Connections::remove(int fd) {
  lock_connections;

  auto it = items.find(fd);
  if (it != items.end()) {
    auto connection = it->second;
    connection->closed = true;
    // delete socket; 通过Connection析构函数来删除
    // 是否要判断在task中也删除？    
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
    task->send(SendSource::none);
    if (task->finished()) {
      removedTasks.push_back(task);
    }
  }

  this->removeTasks();
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