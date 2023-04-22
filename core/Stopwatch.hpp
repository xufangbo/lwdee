#pragma once

#include <sys/time.h>

#include <iostream>

class Stopwatch {
 private:
  uint64_t startTs = 0;

 public:
  Stopwatch() { this->start(); }
  void start();
  float stop();
  float elapsed();
  static uint64_t currentMilliSeconds();
  static float elapsed(uint64_t start);
};