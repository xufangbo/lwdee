#include "Stopwatch.hpp"

#include <sys/timeb.h>

#include <chrono>
#include <iostream>

uint64_t Stopwatch::currentMilliSeconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void Stopwatch::start() { this->startTs = currentMilliSeconds(); }

float Stopwatch::stop() {
  auto now = currentMilliSeconds();
  auto eclapsed = (now - startTs) * 1.0 / 1000;
  startTs = now;

  return eclapsed;
}

float Stopwatch::elapsed() {
  return (currentMilliSeconds() - startTs) * 1.0 / 1000;
}

float Stopwatch::elapsed(uint64_t start) {
  return (currentMilliSeconds() - start) * 1.0 / 1000;
}

// long Stopwatch::currentTs() {

//   time_t t = time(NULL);
//   return t;

//   // std::chrono::system_clock::time_point now =
//   std::chrono::system_clock::now();
//   // time_t timestamp = std::chrono::system_clock::to_time_t(now);
// }