#pragma once

typedef struct {
  /**
   * 总内存M
   */
  unsigned long totalram;
  /**
   * 已使用内存M
   */
  unsigned long usedram;
  /**
   * 未使用内存M
   */
  unsigned long freeram;
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