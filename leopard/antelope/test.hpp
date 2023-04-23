#pragma once

#include <iostream>
#include "LaneClient.hpp"

class TestInput;
class TestReport;
void test_sync(TestReport &testReport,int testSize, TestInput& inputType, std::string ip, int port, float timeout);
void test_async(TestReport &testReport,int testSize, TestInput& inputType, std::string ip, int port, float timeout);
void test_long(TestReport &testReport,int testSize, TestInput& inputType, int parallel, std::string ip, int port, float timeout);

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
  void writeLine(std::string type, std::string inputType, int testSize, int parallel, float elapsed);
};