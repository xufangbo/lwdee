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
  static void running();
  static bool is_running;

 public:
  static double cpu_usage();
  static RamInfo ram_info();
  static void start();
};