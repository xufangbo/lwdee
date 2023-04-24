#include "Metrix.hpp"

#include <unistd.h>
#include <filesystem>
#include <fstream>
#include <numeric>
#include <thread>

#include "core/log.hpp"

void Metrix::start(bool* running, ApplicationType appType, std::vector<Qps*> qpss) {
  this->running = running;
  this->appType = appType;

  if (qpss.size() > 1) {
    this->qpses.push_back(&qps);
  }

  for (Qps* i : qpss) {
    this->qpses.push_back(i);
  }

  std::string dirPath = "./log";
  this->fileName = dirPath + "/" + fileName;

  std::filesystem::directory_entry dir(dirPath);
  if (!dir.exists()) {
    std::filesystem::create_directory(dirPath);
  }

  this->writers.push_back(std::make_shared<CsvMetrixWriter>());
  // this->writers.push_back(std::make_shared<MarkdownMetrixWriter>());
  if (appType == ApplicationType::server) {
    this->writers.push_back(std::make_shared<ConsoleMetrixWriter>());
  }

  for (auto& writer : this->writers) {
    writer->writeTitle(this->fileName, this->qpses);
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
  auto begin = qpses.size() > 1 ? qpses.begin() + 1 : qpses.begin();
  for (auto i = begin; i != qpses.end(); i++) {
    (*i)->generate();
  }

  if (qpses.size() > 1) {
    qps.values.clear();
    int valueSize = qpses[1]->values.size();
    for (int i = 0; i < valueSize; i++) {
      int value = std::accumulate(qpses.begin() + 1, qpses.end(), 0, [&i](uint32_t x, Qps* r) { return x + r->values.at(i); });
      qps.values.push_back(value);
    }
  }

  for (auto& writer : this->writers) {
    writer->writeLine(fileName, qpses);
  }
}

void CsvMetrixWriter::writeTitle(std::string& fileName, std::vector<Qps*>& qpses) {
  std::string file = fileName + ".csv";
  std::ofstream f(file, std::ios_base::trunc);
  if (!f.is_open()) {
    logger_error("can't open file %s", file.c_str());
  }

  if (qpses.size() > 1) {
    f << "" << split;
    f << (qpses.size() - 1) << "线程合计" << split;
    for (int i = 1; i < qpses[0]->header().size(); i++) {
      f << split;
    }
    for (int t = 1; t < qpses.size(); t++) {
      f << "线程" << t << split;
      for (int i = 1; i < qpses[0]->header().size(); i++) {
        f << split;
      }
    }
    f << std::endl;
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
void CsvMetrixWriter::writeLine(std::string& fileName, std::vector<Qps*>& qpses) {
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

void MarkdownMetrixWriter::writeTitle(std::string& fileName, std::vector<Qps*>& qpses) {
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

  fflush(fp);
  fclose(fp);
}
void MarkdownMetrixWriter::writeLine(std::string& fileName, std::vector<Qps*>& qpses) {
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

void ConsoleMetrixWriter::writeTitle(std::string& fileName, std::vector<Qps*>& qpses) {
  // std::string file = fileName + ".md";

  // FILE* fp = fopen(file.c_str(), "w");
  // if (fp == NULL) {
  //   logger_error("can't open file %s", file.c_str());
  // }

  FILE* fp = stdout;

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

  fflush(fp);
  // fclose(fp);
}
void ConsoleMetrixWriter::writeLine(std::string& fileName, std::vector<Qps*>& qpses) {
  // std::string file = fileName + ".md";
  // FILE* fp = fopen(file.c_str(), "a");
  // if (fp == NULL) {
  //   logger_error("can't open file %s", file.c_str());
  // }
  FILE* fp = stdout;

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
  // fclose(fp);
}