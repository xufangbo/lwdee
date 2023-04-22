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
  std::atomic<uint32_t> waitings;       // 4
  std::function<uint32_t()> waiting_fun = nullptr;  // 5

  Qps(int id)
      : id(id) {
    this->reset();
  }

  void reset();
  std::vector<std::string> header();
  std::vector<std::string> data();
};