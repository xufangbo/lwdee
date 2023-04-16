#include "SendTask.hpp"

#include <vector>
#include <thread>

#include "core/log.hpp"

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

    std::vector<SendTask *> removes;

    for (auto it = list.begin(); it != list.end(); it++) {
      SendTask *task = *it;
      auto outputStream = task->outputStream;

      int rc = task->socket->send(task->buffer(), task->leftover());

      if (rc == task->leftover()) {
        removes.push_back(task);
        // logger_trace("send finished");
      } else if (rc == -1) {
      } else {
        task->moveon(rc);
        logger_trace("send %ld / %ld", task->leftover(), task->outputStream->size());
      }
    }

    for(SendTask * task : removes){
      list.remove(task);
      delete task;
    }
  }
}