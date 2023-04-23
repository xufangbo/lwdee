#pragma once

#include <iostream>
#include "LaneClient.hpp"

class TestInput;
class TestReport;

class TestInput {
 public:
  int size = 0;
  std::string name;
  std::string value;

 public:
  TestInput(std::string name, int size)
      : name(name), size(size), value(size, 'x') {}
  std::string& operator()() {
    return value;
  }
};

class TestReport {
 private:
  std::string fileName = "./log/test_report.csv";
  char split = ',';
  int seq = 0;

 public:
  void writeTitle();
  void writeEmptyLine();
  void writeLine(std::string type, std::string inputType, int testSize, int parallel, float elapsed);
};

class Testor {
 public:
  std::string name;
  Testor(std::string name)
      : name(name) {}
  void operator()(TestReport& testReport, int testSize, TestInput& inputType, int parallel, std::string ip, int port, float timeout) {
    this->execute(testReport, testSize, inputType, parallel, ip, port, timeout);
  }
  virtual void execute(TestReport& testReport, int testSize, TestInput& inputType, int parallel, std::string ip, int port, float timeout) = 0;
};

class TestSync : public Testor {
 public:
  TestSync()
      : Testor("短连接串行") {}
  void execute(TestReport& testReport, int testSize, TestInput& inputType, int parallel, std::string ip, int port, float timeout) override;
};

class TestAsync : public Testor {
 public:
  TestAsync()
      : Testor("短连接并行") {}
  void execute(TestReport& testReport, int testSize, TestInput& inputType, int parallel, std::string ip, int port, float timeout) override;
};

class TestLongConnect : public Testor {
 public:
  TestLongConnect()
      : Testor("长连接并行") {}
  void execute(TestReport& testReport, int testSize, TestInput& inputType, int parallel, std::string ip, int port, float timeout) override;
};