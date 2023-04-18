#pragma once

#include <signal.h>
#include <numeric>
#include "IRunway.hpp"
#include "log.hpp"
#include "sys/sysinfo.h"

template <class T>
class IRunwayContainer {
 protected:
  bool running = false;
  std::vector<T*> runways;
  std::vector<SendTaskQueue*> sends;

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

    for (int i = 0; i < p2; i++) {
      auto sendQueue = new SendTaskQueue(i + 1);
      sendQueue->start(&running);

      sends.push_back(sendQueue);
    }

    for (int i = 0; i < p1; i++) {
      auto sendQueue = sends[i % sends.size()];
      this->newInstance(i + 1, &running, sendQueue);
    }

    for (auto runway : this->runways) {
      runway->start();
    }

    std::thread tps(&IRunwayContainer::qpsJob, this);
    tps.detach();
  }

  void stop() { this->running = false; }

  void join() {
    for (auto i : runways) {
      i->join();
    }
  }

 protected:
  virtual void newInstance(int id, bool* running, SendTaskQueue* sendQueue) = 0;

  void qpsJob() {
    while (running) {
      sleep(1);

      Qps qps(0);

      qps.accepts = std::accumulate(runways.begin(), runways.end(), 0, [](int x, IRunway* r) { return x + r->qps()->accepts; });
      qps.closes = std::accumulate(runways.begin(), runways.end(), 0, [](int x, IRunway* r) { return x + r->qps()->closes; });
      qps.inputs = std::accumulate(runways.begin(), runways.end(), 0, [](int x, IRunway* r) { return x + r->qps()->inputs; });
      qps.outputs = std::accumulate(runways.begin(), runways.end(), 0, [](int x, IRunway* r) { return x + r->qps()->outputs; });
      int waitings = std::accumulate(runways.begin(), runways.end(), 0, [](int x, IRunway* r) { return x + r->qps()->waitings(); });

      // logger_trace("sockets:%4d,TPS:%4d,epoll wait:%4d",sockets, tps, waits);
    }
  }
};