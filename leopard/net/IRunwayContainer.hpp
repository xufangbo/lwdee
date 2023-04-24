#pragma once

#include <signal.h>
#include <sys/sysinfo.h>

#include "IRunway.hpp"
#include "Metrix.hpp"
#include "log.hpp"
#include "enums.hpp"

template <class T>
class IRunwayContainer {
 protected:
  int parallel = 1;
  bool server = false;
  bool running = false;
  std::vector<T*> runways;
  Metrix matrix;
  ApplicationType appType = ApplicationType::server;

 public:
  ~IRunwayContainer() {
    for (int i = 0; i < runways.size(); i++) {
      delete runways[i];
    }
    runways.clear();
  }

  void start(ApplicationType appType, int parallel = 1) {
    this->appType = appType;
    if (parallel <= 0) {
      this->parallel = get_nprocs();  // get_nprocs_conf();
    }
    // https://blog.csdn.net/weixin_33196646/article/details/116730365
    signal(SIGPIPE, SIG_IGN);
    if (running) {
      return;
    }

    this->running = true;

    leopard_info("parallel: %d", parallel);

    for (int i = 0; i < parallel; i++) {
      this->newInstance(i + 1, &running);
    }

    for (auto runway : this->runways) {
      runway->start();
    }

    std::vector<Qps*> qps;
    for (auto runway : runways) {
      qps.push_back(runway->qps());
    }
    matrix.start(&running, this->appType, qps);
  }

  void stop() { this->running = false; }

  void join() {
    for (auto i : runways) {
      i->join();
    }
  }

 protected:
  virtual void newInstance(int id, bool* running) = 0;
};