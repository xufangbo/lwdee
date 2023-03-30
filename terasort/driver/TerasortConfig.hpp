#pragma once

#include <iostream>

class TerasortConfig {
 private:
  static TerasortConfig _instance;

 public:
  // terasort
  std::string inputFile;
  std::string outputFile;
  int datum = 0;
  int splitNums1 = 0;
  int splitNums2 = 0;
  int is_matrix = 0;

 public:
  void readConfig();
  static TerasortConfig* instance();
};
