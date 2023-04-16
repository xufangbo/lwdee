#include "Leopard.hpp"

#include <signal.h>

#include <numeric>

#include "core/Exception.hpp"
#include "core/log.hpp"
#include "net/Epoll.hpp"
#include "net/Socket.h"
#include "sys/sysinfo.h"

Leopard::Leopard(int corenums) : corenums(corenums) {
  if (corenums <= 0) {
    this->corenums = get_nprocs();  // get_nprocs_conf();
  }
}

Leopard::~Leopard() {
  for (int i = 0; i < corenums; i++) {
    delete runways[i];
  }
  runways.clear();
}

void Leopard::start(std::string ip, int port) {
  // https://blog.csdn.net/weixin_33196646/article/details/116730365
  signal(SIGPIPE, SIG_IGN);

  this->ip = ip;
  this->port = port;

  sendQueue.start();

  logger_info("%s:%d,corenums:%d", ip.c_str(), port, corenums);

  for (int i = 0; i < corenums; i++) {
    auto runway = new Runway(&sendQueue);
    runway->start(ip, port, i + 1);

    runways.push_back(runway);
  }

  tpsThread = std::thread(&Leopard::tpsJob, this);

  for (auto runway : runways) {
    runway->join();
  }
  tpsThread.join();
}

/**
 * TPS统计，系统每秒钟能够处理的业务数量
 * 是一种业务概念，不是一种标准，不同的业务人员统计口径可能不一样
 * 比如可能指的是每秒钟能保存成功的订单数
 * 这里指的是一秒钟RPC请求处理的个数
 *
 * QPS
 */
void Leopard::tpsJob() {
  for (;;) {
    sleep(1);

    int sockets =
        std::accumulate(runways.begin(), runways.end(), 0,
                        [](int x, Runway *r) { return x + r->sockets(); });
    int tps = std::accumulate(runways.begin(), runways.end(), 0,
                              [](int x, Runway *r) { return x + r->tps(); });
    int waits =
        std::accumulate(runways.begin(), runways.end(), 0,
                        [](int x, Runway *r) { return x + r->waits(); });

    // logger_trace("sockets:%4d,TPS:%4d,epoll wait:%4d",sockets, tps, waits);
  }
}