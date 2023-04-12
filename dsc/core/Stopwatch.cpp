#include "Stopwatch.h"

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

  return eclipse * 1.0 / 1000;
}

long Stopwatch::currentMilliSeconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}