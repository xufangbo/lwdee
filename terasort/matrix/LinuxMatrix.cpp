#include "LinuxMatrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <thread>
#include "core/log.hpp"
#include "cpu.h"

bool LinuxMatrix::is_running = false;

void matrix() {
  while (true) {
    auto cpu = LinuxMatrix::cpu_usage();
    auto ram = LinuxMatrix::ram_info();

    logger_trace("cpu: %.2f%%,ram total: %ldM,available: %ldM,used: %ldM,free: %ldM", cpu, ram.total, ram.available, (ram.total - ram.available), ram.free);
    // printf("cpu rate : %lf /%,ram total: %ld M, used: %ld M, free: %ld M\n", cpu, ram.totalram, ram.usedram, ram.freeram);

    sleep(1);
  }
}

void LinuxMatrix::start() {
  if (is_running) {
    return;
  }
  // auto process = []() {};
  auto t1 = new std::thread(matrix);

  is_running = true;
}

double LinuxMatrix::cpu_usage() {
  return get_sysCpuUsage();
}

RamInfo LinuxMatrix::ram_info() {
  RamInfo ram;

  struct sysinfo s_info;
  if (sysinfo(&s_info) == 0) {
    ram.total = s_info.totalram / 1024 / 1024;
    ram.bufferr = s_info.bufferram / 1024 / 1024;
    ram.free = s_info.freeram / 1024 / 1024;
    ram.available = ram.bufferr + ram.free;
  } else {
    logger_trace("can't get ram");
  }

  return ram;
}
