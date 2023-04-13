#pragma once

#include <atomic>
#include <iostream>
#include <mutex>

typedef struct {
  /**
   * 总内存M
   */
  unsigned long total;
  /**
   * 已使用内存M
   */
  unsigned long used;
  /**
   * 未使用内存M
   */
  unsigned long free;

  /**
   * IO缓存
   */
  unsigned long bufferr;

  /**
   * @brief 可用内存
   *  free + bufferd
   */
  unsigned long available;
} RamInfo;

class StreamMatrix {
 public:
  StreamMatrix() {
    kafka_dco = 0;
    map_dco = 0;
    reduce_dco = 0;
    kafka_send = 0;
    map_accept = 0;
    reduce_accept = 0;
  }
  std::atomic<int> kafka_dco;
  std::atomic<int> map_dco;
  std::atomic<int> reduce_dco;

  std::atomic<uint64_t> kafka_send;
  std::atomic<uint64_t> map_accept;
  std::atomic<uint64_t> reduce_accept;
};

class LinuxMatrix {
 private:
  static std::mutex mut;
  static bool is_running;
  static double cpu_bottom;
  static double cpu_top;
  static unsigned long ram_bottom;
  static unsigned long ram_top;

  static double cpu;
  static RamInfo ram;

 public:
  static StreamMatrix stream;

 private:
  static void running();

 public:
  static void collect(double cpu, RamInfo& ram);
  static double cpu_usage();
  static RamInfo ram_info();
  static void start();
  static void print();
};