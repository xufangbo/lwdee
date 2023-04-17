#include "matrix.h"
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <sys/sysinfo.h>  // for get_nprocs
#include <unistd.h>       // for getpid and usleep

// cpu信息的结构体，读文件/proc/stat
// see: https://man7.org/linux/man-pages/man5/proc.5.html
struct CpuStat {
  char name[8];          // 设备名，一般值为pu
  unsigned long user;    // 用户态的CPU时间（单位:0.01s）
  unsigned long nice;    // nice值为负的进程所占用的CPU时间（单位:0.01s）
  unsigned long system;  // 处于核心态的运行时间（单位:0.01s）
  unsigned long idle;    // 除IO等待时间意外其他等待时间（单位:0.01s）
};

// 内存信息的结构体，读文件/proc/meminfo
struct MemInfo {
  unsigned long total;      // MemTotal 总内存
  unsigned long free;       // MemFree  空闲内存
  unsigned long avaliable;  // MemAvailable 可用内存，约等于 MemFree + Buffer + Catch
};

// 解析/proc/stat文件，获取CPU信息
static void GetSystemCpuStat(CpuStat* s) {
  memset(s, 0, sizeof(CpuStat));

  char buf[256];
  FILE* fd = fopen("/proc/stat", "r");
  assert(fd != nullptr);
  fgets(buf, sizeof(buf), fd);
  sscanf(buf, "%s  %lu %lu %lu %lu", s->name, &s->user, &s->nice, &s->system, &s->idle);
  fclose(fd);
  // fprintf(stdout, "%s: %lu %lu %lu %lu\n", s->name, s->user, s->nice, s->system, s->idle);
}

// 向后偏移14项，正好是utime
#define PROCESS_ITEM 14

// 获取/proc/{pid}/stat中CPU的使用信息，主要是
// (14) utime  %lu
// (15) stime  %lu
// (16) cutime  %ld
// (17) cstime  %ld
static unsigned long GetProcessCpuTime(int pid) {
  // 计算当前进程对应的文件名
  char filename[64];
  snprintf(filename, sizeof(filename) - 1, "/proc/%d/stat", pid);

  char buf[1024];
  FILE* fd = fopen(filename, "r");
  assert(fd != nullptr);
  fgets(buf, sizeof(buf), fd);
  // fprintf(stdout, "%s: %s", filename, buf);
  fclose(fd);

  // 向后偏移，直至utime项
  int count = 1;
  const char* p = buf;
  while (*p != '\0') {
    if (*p++ == ' ') {
      count++;
      if (count == PROCESS_ITEM)
        break;
    }
  }
  if (*p == '\0')
    return 0;

  unsigned long utime = 0;   // user time
  unsigned long stime = 0;   // kernel time
  unsigned long cutime = 0;  // all user time
  unsigned long cstime = 0;  // all dead time
  sscanf(p, "%ld %ld %ld %ld", &utime, &stime, &cutime, &cstime);
  // fprintf(stdout, "proc[%d]: %lu %lu %lu %lu\n", pid, utime, stime, cutime, cstime);
  return (utime + stime + cutime + cstime);
}

// 获取CPU利用率，如CPU利用率为60%，则返回60
CpuUsed GetCpuUsed(int ms) {
  CpuUsed used;
  used.sys_used = used.proc_used = 0;

  int pid = getpid();
  CpuStat s1, s2;
  GetSystemCpuStat(&s1);
  unsigned long proc_time1 = GetProcessCpuTime(pid);

  usleep(ms * 1000);

  GetSystemCpuStat(&s2);
  unsigned long proc_time2 = GetProcessCpuTime(pid);

  // 计算CPU利用率：使用时间/总时间
  unsigned int sys_used1 = s1.user + s1.nice + s1.system;
  unsigned int sys_used2 = s2.user + s2.nice + s2.system;
  unsigned int sys_total1 = sys_used1 + s1.idle;
  unsigned int sys_total2 = sys_used2 + s2.idle;

  if (sys_total1 < sys_total2) {
    used.sys_used = 100 * (sys_used2 - sys_used1) / (sys_total2 - sys_total1);
    // 多线程，需要乘CPU core数量
    used.proc_used = get_nprocs() * 100 * (proc_time2 - proc_time1) / (sys_total2 - sys_total1);
  }

  return used;
}

// 解析/proc/meminfo文件，获取内存信息
// MemTotal %lu
// MemFree %lu
// MemAvailable %lu (since Linux 3.14)
static void GetSystemMeminfo(MemInfo* info) {
  memset(info, 0, sizeof(MemInfo));

  char buf[128];
  char name[64];
  char unit[64];
  FILE* fd = fopen("/proc/meminfo", "r");
  assert(fd != nullptr);
  fgets(buf, sizeof(buf), fd);
  sscanf(buf, "%s %u %s", name, &info->total, unit);
  fgets(buf, sizeof(buf), fd);
  sscanf(buf, "%s %u %s", name, &info->free, unit);
  fgets(buf, sizeof(buf), fd);
  sscanf(buf, "%s %u %s", name, &info->avaliable, unit);
  fclose(fd);
  // fprintf(stdout, "mem: %u %u %u\n", info->total, info->free, info->avaliable);
}

// 解析/proc/{pid}/statm文件，获取内存信息，前两项为
// size (pages) 任务虚拟地址空间的大小 VmSize/4，单位 KB
// resident(pages) 应用程序正在使用的物理内存的大小 VmRSS/4，单位 KB
static unsigned long GetProcessMeminfo(int pid) {
  // 计算当前进程对应的文件名
  char filename[64];
  snprintf(filename, sizeof(filename) - 1, "/proc/%d/statm", pid);

  char buf[256];
  FILE* fd = fopen(filename, "r");
  assert(fd != nullptr);
  fgets(buf, sizeof(buf), fd);
  fclose(fd);

  unsigned long size = 0;
  unsigned long resident = 0;
  sscanf(buf, "%lu %lu", &size, &resident);
  // fprintf(stdout, "proc mem: %u %u\n", size, resident);
  return resident * 4;
}

// 获取系统已使用的内存，单位MB。total为输出参数，表示总内存
int GetSystemMemUsed(int& total) {
  MemInfo info;
  GetSystemMeminfo(&info);
  total = info.total / 1024;
  return (total - info.avaliable / 1024);
}

// 获取本进程已使用的内存，单位MB
int GetProcessMemUsed(void) {
  unsigned long mem_used = GetProcessMeminfo(getpid());
  return ((mem_used < 1024) ? 1 : mem_used / 1024);
}

int main() {
  for (int i = 0; i < 10; i++) {
    auto used = GetCpuUsed();
    printf("cpu used , sys %ld, proc : %ld\n", used.sys_used, used.proc_used);

    int ram_total;
    int ram_used = GetSystemMemUsed(ram_total);
    int ram_proc = GetProcessMemUsed();

    printf("ram total: %dM, os used: %dM, current proc used: %dM\n", ram_total, ram_used, ram_proc);

    printf("------------\n");

    sleep(1);
  }

  return 0;
}
