#pragma once

#include <numeric>
#include "IRunway.hpp"

template <class T>
class IRunwayContainer {
 protected:
  bool running = false;
  std::vector<T*> runways;
  SendTaskQueue sendQueue;

 public:
  ~IRunwayContainer() {
    for (int i = 0; i < runways.size(); i++) {
      delete runways[i];
    }
    runways.clear();
  }

  void stop() { this->running = false; }

  void join() {
    for (auto i : runways) {
      i->join();
    }
  }

 protected:
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