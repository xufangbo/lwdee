#pragma once

#include <iostream>

#include "core/Bytes10.h"

typedef std::pair<Bytes10, Bytes10> MinAndMax;

class Driver {
 private:
  MinAndMax samples(std::string fileName, int datum);

 public:
  void startJob(std::string fileName, int datum, int splitNum1, int splitNum2);
};