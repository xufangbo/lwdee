#pragma once

#include <atomic>
#include <functional>
#include <iostream>
#include <vector>

struct Qps {
  int id;
  std::atomic<uint32_t> sockets = 0;  // 新建连接数
  std::atomic<uint32_t> closes = 0;   // 关闭连接数
  std::atomic<uint32_t> recvs = 0;    // 收到的请求
  std::atomic<uint32_t> sends = 0;    // 发送数
  std::atomic<uint32_t> bullets = 0;  // 发送中的数

  uint32_t sockets_ = 0;  // 上次新建连接数
  uint32_t closes_ = 0;   // 上次关闭连接数
  uint32_t recvs_ = 0;    // 上次收到的请求
  uint32_t sends_ = 0;    // 上次发送数
  uint32_t bullets_ = 0;  // 上次发送中的数

  std::vector<uint32_t> values;

  Qps(int id)
      : id(id) {
    this->reset();
  }

  void reset();
  void generate();
  std::vector<std::string> header();
  std::vector<std::string> data();
};