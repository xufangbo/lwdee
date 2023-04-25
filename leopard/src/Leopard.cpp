#include "Leopard.hpp"

Leopard::~Leopard() {
  for (int i = 0; i < runways.size(); i++) {
    delete runways[i];
  }
  runways.clear();
}

void Leopard::start(std::string ip, int port, int parallel) {
  this->appType = ApplicationType::server;
  if (parallel <= 0) {
    this->parallel = get_nprocs();  // get_nprocs_conf();
  }
  // https://blog.csdn.src/weixin_33196646/article/details/116730365
  signal(SIGPIPE, SIG_IGN);
  if (running) {
    return;
  }

  this->running = true;

  leopard_info("parallel: %d", parallel);

  for (int i = 0; i < parallel; i++) {
    auto runway = new Runway(i + 1, &running, ip, port);
    this->runways.push_back(runway);
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

void Leopard::start(int parallel) {
  this->appType = ApplicationType::client;
  if (parallel <= 0) {
    this->parallel = get_nprocs();  // get_nprocs_conf();
  }
  // https://blog.csdn.src/weixin_33196646/article/details/116730365
  signal(SIGPIPE, SIG_IGN);
  if (running) {
    return;
  }

  this->running = true;

  leopard_info("parallel: %d", parallel);

  for (int i = 0; i < parallel; i++) {
    auto runway = new Runway(i + 1, &running);
    this->runways.push_back(runway);
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

void Leopard::stop() {
  this->running = false;
}

void Leopard::join() {
  for (auto i : runways) {
    i->join();
  }
}

Leopard Leopard::instance;

ClientConnection* Leopard::create(std::string ip, int port) {
  auto runway = std::min_element(runways.begin(), runways.end(), [](Runway* x, Runway* y) { return x->size() - y->size(); });
  if (runway == runways.end()) {
    throw Exception("ERROR", ZONE);
  }

  ClientConnection* skt = (*runway)->create(ip, port);
  return skt;
}