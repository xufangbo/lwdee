#include "Metrix.hpp"

#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <numeric>
#include <thread>

#include "core/log.hpp"

void Metrix::start(bool* running, std::string fileName,
                   std::vector<Qps*> qpses) {
  this->running = running;
  this->qpses = qpses;

  std::string dirPath = "./log";
  this->fileName = dirPath + "/" + fileName;

  std::filesystem::directory_entry dir(dirPath);
  if (!dir.exists()) {
    std::filesystem::create_directory(dirPath);
  }

  this->writers.push_back(std::make_shared<CsvMetrixWriter>());
  this->writers.push_back(std::make_shared<MarkdownMetrixWriter>());

  for (auto& writer : this->writers) {
    writer->writeTitle(this->fileName, qpses);
  }

  std::thread t(&Metrix::run, this);
  t.detach();
}

void Metrix::run() {
  while (*running) {
    this->write();
    sleep(1);
  }
}

void Metrix::write() {
  // Qps qps(0);
  // qps.accepts = std::accumulate(qpses.begin(), qpses.end(), 0, [](int x, Qps*
  // r) { return x +r->accepts; }); qps.closes = std::accumulate(qpses.begin(),
  // qpses.end(), 0, [](int x, Qps* r) { return x +r->closes; }); qps.inputs =
  // std::accumulate(qpses.begin(), qpses.end(), 0, [](int x, Qps* r) { return x
  // +r->inputs; }); qps.outputs = std::accumulate(qpses.begin(), qpses.end(),
  // 0, [](int x, Qps* r) { return x +r->outputs; }); int waitings =
  // std::accumulate(qpses.begin(), qpses.end(), 0, [](int x, Qps* r) { return x
  // +r->waitings(); });

  for (auto& writer : this->writers) {
    writer->writeLine(fileName, qpses);
  }

  for (Qps* qps : qpses) {
    auto tmp = qps->data();
    qps->reset();
  }
}

void CsvMetrixWriter::writeTitle(std::string& fileName,
                                 std::vector<Qps*>& qpses) {
  std::string file = fileName + ".csv";
  std::ofstream f(file, std::ios_base::trunc);
  if (!f.is_open()) {
    logger_error("can't open file %s", file.c_str());
  }

  f << "time" << split;
  for (Qps* qps : qpses) {
    auto tmp = qps->header();
    for (std::string& i : tmp) {
      f << i << split;
    }
  }
  f << std::endl;

  f.flush();
  f.close();
}
void CsvMetrixWriter::writeLine(std::string& fileName,
                                std::vector<Qps*>& qpses) {
  std::string file = fileName + ".csv";
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

void MarkdownMetrixWriter::writeTitle(std::string& fileName,
                                      std::vector<Qps*>& qpses) {
  std::string file = fileName + ".md";

  FILE* fp = fopen(file.c_str(), "w");
  if (fp == NULL) {
    logger_error("can't open file %s", file.c_str());
  }

  fprintf(fp, "|% 23s|", "time");

  for (Qps* qps : qpses) {
    auto tmp = qps->header();
    for (std::string& i : tmp) {
      fprintf(fp, "% 6s |", i.c_str());
    }
  }
  fprintf(fp, "\n");

  //-----------------------------
  fprintf(fp, "|%23s|", "-----------------------");

  for (Qps* qps : qpses) {
    auto tmp = qps->header();
    for (std::string& i : tmp) {
      fprintf(fp, "%6s-|", "------");
    }
  }
  fprintf(fp, "\n");

  // //-----------------------------
  // fprintf(fp, "|%23s|", "           -           ");

  // for (int i = 0; i < qpses.size(); i++) {
  //   Qps* qps = qpses[i];
  //   auto tmp = qps->header();
  //   for (std::string& s : tmp) {
  //     fprintf(fp, "  %02d   |", i + 1);
  //   }
  // }
  // fprintf(fp, "\n");

  fflush(fp);
  fclose(fp);
}
void MarkdownMetrixWriter::writeLine(std::string& fileName,
                                     std::vector<Qps*>& qpses) {
  std::string file = fileName + ".md";
  FILE* fp = fopen(file.c_str(), "a");
  if (fp == NULL) {
    logger_error("can't open file %s", file.c_str());
  }

  char time[25];
  date_millsecond(time, 25);
  fprintf(fp, "|%23s|", time);

  for (Qps* qps : qpses) {
    auto tmp = qps->data();
    for (std::string& i : tmp) {
      fprintf(fp, "%+6s |", i.c_str());
    }
  }
  fprintf(fp, "\n");

  fflush(fp);
  fclose(fp);
}