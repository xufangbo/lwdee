#pragma once

#include <iostream>

class LeopardConfig {
 private:
  static LeopardConfig _instance;

 public:
  // terasort
  std::string ip;
  int port = 0;
  int sparallel = 0;
  int cparallel = 0;

 public:
  void readConfig();
  static LeopardConfig* instance();
};
