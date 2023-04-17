#include "LinuxMatrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <fstream>
#include <thread>
#include "core/DscConfig.hpp"
#include "core/log.hpp"
#include "cpu.h"

std::mutex LinuxMatrix::mut;
std::string LinuxMatrix::fileName;
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
    sleep(1);
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

  std::ofstream f(fileName, std::ios_base::app);
  if (!f.is_open()) {
    logger_trace("can't open file %s", fileName.c_str());
    return;
  }

  char time[25];
  date_millsecond(time, 25);
  time[19] = ' ';

  f << time << ",";

  f << stream.kafka_index << ",";
  f << stream.kafka_qps << ",";
  f << stream.kafka_send << ",";
  f << stream.map_dco << ",";
  f << stream.map_accept << ",";
  f << stream.reduce_dco << ",";
  f << stream.reduce_accept << ",";

  f << cpu_bottom << ",";
  f << cpu_top << ",";
  f << (cpu_top - cpu_bottom) << ",";

  f << ram_bottom << ",";
  f << ram_top << ",";
  f << (ram_top - ram_bottom) << ",";

  f << ram.available << ",";
  f << (ram.total - ram.available) << ",";
  f << ram.free << ",";

  f << std::endl;

  f.flush();
  f.close();
}

void createFile(std::string fileName) {
  // if (access(fileName.c_str(), 0) == 0) {
  //   if (remove(fileName.c_str()) == 0) {
  //     logger_trace("rm %s", fileName.c_str());
  //   } else {
  //     logger_error("can't rm %s", fileName.c_str());
  //   }
  // }

  std::ofstream f(fileName, std::ios_base::trunc);
  if (!f.is_open()) {
    logger_trace("can't open file %s", fileName.c_str());
  }

  f << "time,";

  f << "kafka index,";
  f << "kafka qps,";
  f << "kafka total,";
  f << "map size,";
  f << "map accept,";
  f << "reduce size,";
  f << "reduce accept,";

  f << "cpu bottom,";
  f << "cpu top,";
  f << "cpu,";

  f << "ram bottom,";
  f << "ram top,";
  f << "ram,";

  f << "ram avai,";
  f << "ram used,";
  f << "ram free";

  f << std::endl;

  f.flush();
  f.close();
}

void LinuxMatrix::start() {
  mut.lock();
  if (is_running) {
    mut.unlock();
    return;
  } else {
    is_running = true;
    auto conf = DscConfig::instance();
    fileName = conf->outputFile + conf->nodeName + "_qps.csv";
    createFile(fileName);

    mut.unlock();
  }

  cpu_bottom = cpu_top = cpu_usage();
  ram_bottom = ram_top = ram_info().used;

  std::thread t1(matrix);
  t1.detach();

  std::thread t2(print_task);
  t2.detach();
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
