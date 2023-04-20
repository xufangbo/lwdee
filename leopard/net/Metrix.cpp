#include "Metrix.hpp"

#include <unistd.h>
#include <filesystem>
#include <fstream>
#include <numeric>
#include <thread>
#include "core/log.hpp"

void Metrix::start(bool* running, std::string fileName, std::vector<Qps*> qpses) {
  this->running = running;
  this->qpses = qpses;

  std::string dirPath = "./data";
  this->fileName = dirPath + "/" + fileName;

  std::filesystem::directory_entry dir(dirPath);
  if (!dir.exists()) {
    std::filesystem::create_directory(dirPath);
  }

  this->__start(',');
  this->__start('|');

  std::thread t(&Metrix::run, this);
  t.detach();
}

void Metrix::__start(char split) {
  std::string file = fileName + (split == ',' ? ".csv" : ".md");
  std::ofstream f(file, std::ios_base::trunc);
  if (!f.is_open()) {
    logger_error("can't open file %s", file.c_str());
  }

  // if (split == '|') {
  //   f << "|";
  // }
  // f << " " << split;
  // for (int i = 0; i < qpses.size(); i++) {
  //   Qps* qps = qpses[i];
  //   auto tmp = qps->header();
  //   for (std::string& item : tmp) {
  //     f << (i + 1) << split;
  //   }
  // }
  // f << std::endl;

  if (split == '|') {
    f << "|";
  }
  f << "time" << split;
  for (Qps* qps : qpses) {
    auto tmp = qps->header();
    for (std::string& i : tmp) {
      f << i << split;
    }
  }
  f << std::endl;

  // -----------------------
  if (split == '|') {
    f << "|-|";
    for (Qps* qps : qpses) {
      auto tmp = qps->header();
      for (std::string& i : tmp) {
        f << '-' << split;
      }
    }
    f << std::endl;
  }
  // -----------------------

  f.flush();
  f.close();
}

void Metrix::run() {
  while (*running) {
    this->write();
    sleep(1);
  }
}

void Metrix::write() {
  // Qps qps(0);
  // qps.accepts = std::accumulate(qpses.begin(), qpses.end(), 0, [](int x, Qps* r) { return x +r->accepts; });
  // qps.closes = std::accumulate(qpses.begin(), qpses.end(), 0, [](int x, Qps* r) { return x +r->closes; });
  // qps.inputs = std::accumulate(qpses.begin(), qpses.end(), 0, [](int x, Qps* r) { return x +r->inputs; });
  // qps.outputs = std::accumulate(qpses.begin(), qpses.end(), 0, [](int x, Qps* r) { return x +r->outputs; });
  // int waitings = std::accumulate(qpses.begin(), qpses.end(), 0, [](int x, Qps* r) { return x +r->waitings(); });

  this->__write(',');
  this->__write('|');

  for (Qps* qps : qpses) {
    auto tmp = qps->data();
    qps->reset();
  }
}

void Metrix::__write(char split) {
  std::string file = fileName + (split == ',' ? ".csv" : ".md");
  std::ofstream f(file, std::ios_base::app);
  if (!f.is_open()) {
    logger_error("can't open file %s", file.c_str());
  }

  char time[25];
  date_millsecond(time, 25);
  time[19] = ' ';

  f << time << split;

  for (Qps* qps : qpses) {
    auto tmp = qps->data();
    for (std::string& i : tmp) {
      f << i << split;
    }
  }
  f << std::endl;

  f.flush();
  f.close();
}