#pragma once

#include <signal.h>
#include <sys/sysinfo.h>
#include "IRunway.hpp"
#include "Metrix.hpp"
#include "log.hpp"

template <class T>
class IRunwayContainer {
 protected:
  bool server = false;
  bool running = false;
  std::vector<T*> runways;
  std::vector<SendTaskQueue*> sends;
  Metrix matrix;

 public:
  ~IRunwayContainer() {
    for (int i = 0; i < runways.size(); i++) {
      delete runways[i];
    }
    runways.clear();

    for (int i = 0; i < sends.size(); i++) {
      delete sends[i];
    }
    sends.clear();
  }
  void start(int p1 = 1, int p2 = 1) {
    // if (corenums <= 0) {
    //   this->corenums = get_nprocs();  // get_nprocs_conf();
    // }
    // https://blog.csdn.net/weixin_33196646/article/details/116730365
    signal(SIGPIPE, SIG_IGN);
    if (running) {
      return;
    }

    this->running = true;

    leopard_info("%d/%d", p1, p2);

    // for (int i = 0; i < p2; i++) {
    //   auto sendQueue = new SendTaskQueue(i + 1);
    //   sendQueue->start(&running);

    //   sends.push_back(sendQueue);
    // }

    for (int i = 0; i < p1; i++) {
      // auto sendQueue = sends[i % sends.size()];
      // this->newInstance(i + 1, &running, sendQueue);
      this->newInstance(i + 1, &running, nullptr);
    }

    for (auto runway : this->runways) {
      runway->start();
    }

    std::vector<Qps*> qps;
    for (auto runway : runways) {
      qps.push_back(runway->qps());
    }
    matrix.start(&running, this->server ? "leopard" : "antelope", qps);
  }

  void stop() { this->running = false; }

  void join() {
    for (auto i : runways) {
      i->join();
    }
  }

 protected:
  virtual void newInstance(int id, bool* running, SendTaskQueue* sendQueue) = 0;
};