#pragma once

#include <iostream>

class LeopardConfig {
 private:
  static LeopardConfig _instance;

 public:
  // terasort
  std::string ip;
  int port = 0;
  int procnums = 0;

 public:
  void readConfig();
  static LeopardConfig* instance();
};
