#pragma once

#include <iostream>
#include <sys/time.h>

class Stopwatch {
 private:
  struct timeval startTs;

 public:
  Stopwatch() {
    this->start();
  }
  void start();
  double stop();
  static uint64_t currentMilliSeconds();
  static float elapsed(uint64_t start);
};