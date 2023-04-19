#include "Matrix.hpp"

#include <unistd.h>
#include <filesystem>
#include <fstream>
#include <numeric>
#include <thread>
#include "core/log.hpp"

void Matrix::start(bool* running, std::string fileName, std::vector<Qps*> qpses) {
  this->running = running;
  this->qpses = qpses;

  std::string dirPath = "./data";
  this->fileName = dirPath + "/" + fileName;

  std::filesystem::directory_entry dir(dirPath);
  if (!dir.exists()) {
    std::filesystem::create_directory(dirPath);
  }

  std::fstream f(fileName, std::ios_base::trunc);
  if (!f.is_open()) {
    logger_error("can't open file %s", fileName.c_str());
  }

  for (Qps* qps : qpses) {
    auto tmp = qps->header();
    for (std::string& i : tmp) {
      f << i << ",";
    }
  }
  f << std::endl;

  f.close();

  std::thread t(&Matrix::run, this);
  t.detach();
}

void Matrix::run() {
  while (*running) {
    this->write();
    sleep(1);
  }
}

void Matrix::write() {
  // Qps qps(0);
  // qps.accepts = std::accumulate(qpses.begin(), qpses.end(), 0, [](int x, Qps* r) { return x +r->accepts; });
  // qps.closes = std::accumulate(qpses.begin(), qpses.end(), 0, [](int x, Qps* r) { return x +r->closes; });
  // qps.inputs = std::accumulate(qpses.begin(), qpses.end(), 0, [](int x, Qps* r) { return x +r->inputs; });
  // qps.outputs = std::accumulate(qpses.begin(), qpses.end(), 0, [](int x, Qps* r) { return x +r->outputs; });
  // int waitings = std::accumulate(qpses.begin(), qpses.end(), 0, [](int x, Qps* r) { return x +r->waitings(); });

  std::fstream f(fileName, std::ios_base::app);
  if (!f.is_open()) {
    logger_error("can't open file %s", fileName.c_str());
  }

  for (Qps* qps : qpses) {
    auto tmp = qps->data();
    for (std::string& i : tmp) {
      f << i << ",";
    }
  }
  f << std::endl;

  f.close();
}