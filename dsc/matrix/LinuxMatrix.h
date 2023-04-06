#pragma once

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

class LinuxMatrix {
 private:
  static bool is_running;
  static double cpu_bottom;
  static double cpu_top;
  static unsigned long ram_bottom;
  static unsigned long ram_top;

  static double cpu;
  static RamInfo ram;

 private:
  static void running();

 public:
  static void collect(double cpu, RamInfo& ram);
  static double cpu_usage();
  static RamInfo ram_info();
  static void start();
  static void print();
};