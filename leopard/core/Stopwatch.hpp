#pragma once

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
};