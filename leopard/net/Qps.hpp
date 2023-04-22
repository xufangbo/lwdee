#pragma once

#include <atomic>
#include <functional>
#include <vector>
#include <iostream>

struct Qps {
  int id;
  std::atomic<uint32_t> sockets = 0;     // 1
  std::atomic<uint32_t> closes;        // 2
  std::atomic<uint32_t> recvs;        // 3
  std::atomic<uint32_t> sends;       // 
  std::atomic<uint32_t> bullets;       // 4

  Qps(int id)
      : id(id) {
    this->reset();
  }

  void reset();
  std::vector<std::string> header();
  std::vector<std::string> data();
};