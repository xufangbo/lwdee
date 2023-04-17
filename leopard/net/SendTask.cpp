#include "SendTask.hpp"

#include <thread>
#include <vector>
#include "core/log.hpp"
#include "net/log.hpp"

bool SendTask::send() {
  int rc = socket->send(buffer(), leftover());

  if (rc == leftover()) {
    _finished = true;
    socket->onSend();
    leopard_trace("send finished");
  } else if (rc == -1) {
    if (errno == EINTR || (errno == EAGAIN) || errno == EWOULDBLOCK) {
      // do nothing
       leopard_trace("EAGAIN");
    } else {
      logger_error("socket send error(%d)%s", errno, strerror(errno));
      return false;
    }
  } else {
    leopard_trace("move on to %ld/%ld",leftover(), outputStream->size());
    moveon(rc);
  }
  return true;
}

SendTaskQueue::~SendTaskQueue() {
  while (!list.empty()) {
    auto task = list.back();
    delete task;
    list.pop_back();
  }
}

void SendTaskQueue::push(Socket* socket, BufferStreamPtr outputStream) {
  leopard_trace("accept socket task");
  list.emplace_back(new SendTask(socket, outputStream));
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

  for (SendTask* task : removes) {
    list.remove(task);
    delete task;
  }

  removes.clear();
}