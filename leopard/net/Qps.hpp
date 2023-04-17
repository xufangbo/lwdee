#pragma once

#include <atomic>
#include <functional>
#include <iostream>

struct Qps {
  int id;
  std::atomic<uint32_t> accepts;       // 1
  std::atomic<uint32_t> closes;        // 2
  std::atomic<uint32_t> inputs;        // 3
  std::atomic<uint32_t> outputs;       // 4
  std::function<uint32_t()> waitings;  // 5
  std::atomic<uint32_t> trans_min;     // 6
  std::atomic<uint32_t> trans_max;     // 7
  std::atomic<uint64_t> trans_sum;     // 8

  Qps(int id)
      : id(id) {
    this->reset();
  }

  void reset() {
    accepts = 0;
    closes = 0;
    inputs = 0;
    outputs = 0;
    trans_min = 100000000;
    trans_sum = 0;
    trans_max = 0;
  }

  void time(uint32_t elapsed) {
    trans_min = trans_min < elapsed ? trans_min.load() : elapsed;
    trans_max = trans_max > elapsed ? trans_max.load() : elapsed;
    trans_sum += elapsed;
  }
};