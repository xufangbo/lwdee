#pragma once

#include <iostream>
#include <memory>

#include "Qps.hpp"
#include "core/matrix.h"
#include "enums.hpp"

class IMetrixWriter;
class Metrix {
 private:
  bool* running = nullptr;
  std::string fileName;
  std::vector<Qps*> qpses;
  std::vector<std::shared_ptr<IMetrixWriter>> writers;
  Qps qps;
  SysResource sysres;
  ApplicationType appType;

 public:
  Metrix()
      : qps(0) {}
  void start(bool* running, ApplicationType appType, std::vector<Qps*> qpses);
  void write();
  void run();
};

class IMetrixWriter {
 public:
  virtual void writeTitle(std::string& fileName, std::vector<Qps*>& qpses) = 0;
  virtual void writeLine(std::string& fileName, std::vector<Qps*>& qpses,SysResource &sysres) = 0;
};

class CsvMetrixWriter : public IMetrixWriter {
 private:
  char split = ',';

 public:
  void writeTitle(std::string& fileName, std::vector<Qps*>& qpses) override;
  void writeLine(std::string& fileName, std::vector<Qps*>& qpses,SysResource &sysres) override;
};
class MarkdownMetrixWriter : public IMetrixWriter {
 private:
  char split = '|';

 public:
  void writeTitle(std::string& fileName, std::vector<Qps*>& qpses) override;
  void writeLine(std::string& fileName, std::vector<Qps*>& qpses,SysResource &sysres) override;
};

class ConsoleMetrixWriter : public IMetrixWriter {
 private:
  char split = '|';

 public:
  void writeTitle(std::string& fileName, std::vector<Qps*>& qpses) override;
  void writeLine(std::string& fileName, std::vector<Qps*>& qpses,SysResource &sysres) override;
};