#include "SendTaskQueue.hpp"

#include <algorithm>
#include <thread>
#include <vector>
#include "core/Stopwatch.hpp"
#include "core/log.hpp"
#include "net/log.hpp"

SendTaskQueue::~SendTaskQueue() {
  while (!list.empty()) {
    auto task = list.back();
    delete task;
    list.pop_back();
  }
}

void SendTaskQueue::push(Socket* socket, BufferStreamPtr outputStream) {
  // std::lock_guard<std::mutex> lock(mut);

  auto it = std::find_if(list.begin(), list.end(), [&socket](SendTask* i) { return i->socket == socket; });

  if (it == list.end()) {
    // leopard_trace("new SendTask");
    list.emplace_back(new SendTask(socket, outputStream));
  } else {
    // leopard_trace("push to SendTask");
    (*it)->push(outputStream);
  }
}

void SendTaskQueue::start(bool* running) {
  this->running = running;
  std::thread t(&SendTaskQueue::run, this);
  t.detach();
}

void SendTaskQueue::run() {
  while (*running) {
    try {
      this->doRun();
    } catch (std::exception& ex) {
      logger_error("%s", ex.what());
    }
  }
}

void SendTaskQueue::doRun() {
  if (list.empty()) {
    usleep(1000);
    return;
  }

  for (auto it = list.begin(); it != list.end(); it++) {
    SendTask* task = *it;
    task->send();
    if (task->finished()) {
      removes.push_back(task);
    }
  }

  this->remove();
}

void SendTaskQueue::remove() {
  // std::lock_guard<std::mutex> lock(mut);

  for (SendTask* task : removes) {
    list.remove(task);
    delete task;
    task = nullptr;
  }

  removes.clear();
}