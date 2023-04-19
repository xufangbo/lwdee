#pragma once

#include <atomic>
#include <functional>
#include <vector>

struct Qps {
  int id;
  std::atomic<uint32_t> accepts;       // 1
  std::atomic<uint32_t> closes;        // 2
  std::atomic<uint32_t> inputs;        // 3
  std::function<uint32_t()> waitings;  // 5

  std::atomic<uint32_t> outputs;    // 4
  std::atomic<uint32_t> trans_min;  // 6
  std::atomic<uint32_t> trans_max;  // 7
  std::atomic<uint64_t> trans_sum;  // 8

  Qps(int id)
      : id(id) {
    this->reset();
  }

  void reset();
  void time(uint32_t elapsed);
  std::vector<std::string> header();
  std::vector<std::string> data();
};