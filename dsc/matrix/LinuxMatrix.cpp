#include "LinuxMatrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <thread>
#include "core/DscConfig.hpp"
#include "core/log.hpp"
#include "cpu.h"

std::mutex LinuxMatrix::mut;
bool LinuxMatrix::is_running = false;
double LinuxMatrix::cpu_bottom = 0;
double LinuxMatrix::cpu_top = 0;
unsigned long LinuxMatrix::ram_bottom = 0;
unsigned long LinuxMatrix::ram_top = 0;
double LinuxMatrix::cpu = 0;
RamInfo LinuxMatrix::ram;
StreamMatrix LinuxMatrix::stream;

void matrix() {
  while (true) {
    auto cpu = LinuxMatrix::cpu_usage();
    auto ram = LinuxMatrix::ram_info();
    LinuxMatrix::collect(cpu, ram);
    // sleep(1);
    usleep(1000000 / 10);
  }
}

void print_task() {
  while (true) {
    LinuxMatrix::print();
    sleep(2);
  }
}

void LinuxMatrix::collect(double cpu, RamInfo& ram) {
  cpu_bottom = cpu_bottom > cpu ? cpu : cpu_bottom;
  cpu_top = cpu_top > cpu ? cpu_top : cpu;

  ram_bottom = ram_bottom > ram.used ? ram.used : ram_bottom;
  ram_top = ram_top > ram.used ? ram_top : ram.used;

  LinuxMatrix::cpu = cpu;
  LinuxMatrix::ram = ram;
}

bool color_change = true;
void LinuxMatrix::print() {
  int d1 = stream.kafka_dco;
  int d2 = stream.kafka_send;
  int d3 = stream.map_dco;
  uint64_t d4 = stream.map_accept;
  uint64_t d5 = stream.reduce_dco;
  uint64_t d6 = stream.reduce_accept;

  if (color_change) {
    logger_trace("CPU(%.2f%%-%.2f%%=%.2f%%),RAM(%ldM-%ldM=%ldM),avai:%ldM,used:%ldM,free:%ldM ", cpu_bottom, cpu_top, cpu_top - cpu_bottom, cpu, ram_bottom, ram_top, ram_top - ram_bottom, ram.available, (ram.total - ram.available), ram.free);
    logger_trace("kafkas(%d / %lld) maps(%d / %lld) reducs(%d / %lld)", d1, d2, d3, d4, d5, d6);
  } else {
    logger_info("CPU(%.2f%%-%.2f%%=%.2f%%),RAM(%ldM-%ldM=%ldM),avai:%ldM,used:%ldM,free:%ldM ", cpu_bottom, cpu_top, cpu_top - cpu_bottom, cpu, ram_bottom, ram_top, ram_top - ram_bottom, ram.available, (ram.total - ram.available), ram.free);
    logger_info("kafkas(%d / %lld) maps(%d / %lld) reducs(%d / %lld)", d1, d2, d3, d4, d5, d6);
  }
  printf("\n");


  color_change = !color_change;
}

void LinuxMatrix::start() {
  mut.lock();
  if (is_running) {
    return;
  }
  is_running = true;
  mut.unlock();

  cpu_bottom = cpu_top = cpu_usage();
  ram_bottom = ram_top = ram_info().used;

  auto t1 = new std::thread(matrix);
  auto t2 = new std::thread(print_task);
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
    ram.used = ram.total - ram.available;
  } else {
    logger_trace("can't get ram");
  }

  return ram;
}
