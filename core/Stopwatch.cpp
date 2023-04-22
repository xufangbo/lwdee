#include "Stopwatch.hpp"

#include <sys/timeb.h>

#include <chrono>
#include <iostream>

void Stopwatch::start() {
  // time(&startTs);
  gettimeofday(&startTs, NULL);
}

double Stopwatch::stop() {
  // time_t t;
  // time(&t);

  struct timeval tv;
  gettimeofday(&tv, NULL);
  auto eclipse = (tv.tv_sec * 1000 + tv.tv_usec / 1000) - (startTs.tv_sec * 1000 + startTs.tv_usec / 1000);

  startTs = tv;

  return eclipse * 1.0 / 1000;
}

double Stopwatch::elapsed() {
  struct timeval tv;
  gettimeofday(&tv, NULL);

  auto eclipse = (tv.tv_sec * 1000 + tv.tv_usec / 1000) - (startTs.tv_sec * 1000 + startTs.tv_usec / 1000);

  return eclipse * 1.0 / 1000;
}

uint64_t Stopwatch::currentMilliSeconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

float Stopwatch::elapsed(uint64_t start) {
  uint32_t elapsed = Stopwatch::currentMilliSeconds() - start;
  return elapsed * 1.0 / 1000;
}

// long Stopwatch::currentTs() {

//   time_t t = time(NULL);
//   return t;

//   // std::chrono::system_clock::time_point now =
//   std::chrono::system_clock::now();
//   // time_t timestamp = std::chrono::system_clock::to_time_t(now);
// }