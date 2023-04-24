// CPU利用率，包含系统CPU和本进程CPU的利用率
struct CpuUsed {
  unsigned long sys_used;
  unsigned long proc_used;
};

// 获取CPU利用率，如CPU利用率为60%，则返回60。
// 系统CPU数据位于文件/proc/stat中，进程CPU数据位于/proc/{pid}/stat中，
// 文件中只有自系统启动以来的CPU时钟数，利用率需要手动进行统计和计算，
// 默认统计时间间隔为500ms（一般设置在0~1s之间），所以该函数会阻塞ms的时间。
CpuUsed GetCpuUsed(int ms = 500);

// 获取系统已使用的内存，单位MB。total为输出参数，表示总内存
// 系统内存数据位于文件/proc/meminfo中
int GetSystemMemUsed(int& total);

// 获取本进程已使用的内存，单位MB。若不足1MB，返回1
// 进程内存数据位于文件/proc/{pid}/statm中
int GetProcessMemUsed(void);

struct SysResource {
  unsigned long cpu_sys_used;
  unsigned long cpu_proc_used;
  int ram_total;
  int ram_sys_used;
  int ram_proc_used;
};
SysResource get_proc_res();
