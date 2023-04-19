#pragma once

#include <iostream>
#include "Qps.hpp"

class Matrix {
 private:
  bool* running = nullptr;
  std::string fileName;
  std::vector<Qps*> qpses;

 public:
  void start(bool* running, std::string fileName, std::vector<Qps*> qpses);
  void __start(char split);
  void write();
  void __write(char split);
  void run();
};