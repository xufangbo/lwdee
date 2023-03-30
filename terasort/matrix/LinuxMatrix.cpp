#include "LinuxMatrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <thread>
#include "core/log.hpp"
#include "cpu.h"

bool LinuxMatrix::is_running = false;

void LinuxMatrix::start() {
  if (is_running) {
    return;
  }

  auto process = []() {
    while (true) {
      auto cpu = LinuxMatrix::cpu_usage();
      auto ram = LinuxMatrix::ram_info();

      logger_trace("cpu rate : %lf%%, ram total: %ld M, used: %ld M, free: %ld M", cpu, ram.totalram, ram.usedram, ram.freeram);
      // printf("cpu rate : %lf /%,ram total: %ld M, used: %ld M, free: %ld M\n", cpu, ram.totalram, ram.usedram, ram.freeram);

      sleep(1);
    }
  };
  auto t1 = new std::thread(process);

  is_running = true;
}

double LinuxMatrix::cpu_usage() {
  return get_sysCpuUsage();
}

RamInfo LinuxMatrix::ram_info() {
  RamInfo ram;

  struct sysinfo s_info;
  if (sysinfo(&s_info) == 0) {
    ram.totalram = s_info.totalram / 1024 / 1024;
    ram.freeram = s_info.freeram / 1024 / 1024;
    ram.usedram = (s_info.totalram - s_info.freeram) / 1024 / 1024;
  } else {
    logger_trace("can't get ram");
  }

  return ram;
}
