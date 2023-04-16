#include "SendTask.hpp"

#include <thread>
#include <vector>

#include "core/log.hpp"

void SendTask::send() {
  int rc = socket->send(buffer(), leftover());

  if (rc == leftover()) {
    _finished = true;
  } else if (rc == -1) {
  } else {
    moveon(rc);
    logger_trace("send %ld / %ld", leftover(), outputStream->size());
  }
}

void SendTaskQueue::push(Socket *socket, BufferStreamPtr outputStream) {
  //   mut.lock();

  // logger_info("< push");

  list.emplace_back(new SendTask(socket, outputStream));
  // list.emplace(std::move(&task));

  // logger_info("> push");
  //   mut.unlock();
}
void SendTaskQueue::start() {
  std::thread t(&SendTaskQueue::run, this);
  t.detach();
}

void SendTaskQueue::run() {
  while (true) {
    if (list.empty()) {
      usleep(1000);
      continue;
    }
    
    for (auto it = list.begin(); it != list.end(); it++) {
      SendTask *task = *it;
      task->send();
      if (task->finished()) {
        removes.push_back(task);
      }
    }

    for (SendTask *task : removes) {
      list.remove(task);
      delete task;
    }

    removes.clear();
  }
}