#pragma once

#include <atomic>
#include <functional>
#include <vector>
#include <iostream>

struct Qps {
  int id;
  std::atomic<uint32_t> opens = 0;     // 1
  std::atomic<uint32_t> closes;        // 2
  std::atomic<uint32_t> recvs;        // 3
  std::atomic<uint32_t> sends;       // 4
  std::function<uint32_t()> waitings;  // 5

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