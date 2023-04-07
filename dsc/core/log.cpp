#include "log.hpp"
#include <dirent.h>
#include <malloc.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#include <io.h>
#include <windows.h>
#else
#include <errno.h>
#include <error.h>
#include <sys/stat.h>  //mkdir
#include <sys/time.h>
#include <sys/timeb.h>  //timeb
#endif

#define ZONE __FUNCTION__, __FILE__, __LINE__
#define MAX_DEBUG 1024
#define LEVEL_COUNT 6
#define FILE_LENGTH 512

#if defined(_WIN32) || defined(_WIN64)
#define green FOREGROUND_GREEN
#define red FOREGROUND_RED
#define blue FOREGROUND_BLUE
#define white FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define yellow FOREGROUND_RED | FOREGROUND_GREEN
#define gray FOREGROUND_BLUE | FOREGROUND_GREEN
#define purple FOREGROUND_BLUE | FOREGROUND_RED
#else
#define NONE "\033[0m"         // 清除颜色，即之后的打印为正常输出，之前的不受影响
#define BLACK "\033[0;30m"     // 深黑
#define L_BLACK "\033[1;30m"   // 亮黑，偏灰褐
#define RED "\033[0;31m"       // 深红，暗红
#define L_RED "\033[1;31m"     // 鲜红
#define GREEN "\033[0;32m"     // 深绿，暗绿
#define L_GREEN "\033[1;32m"   // 鲜绿
#define BROWN "\033[0;33m"     // 深黄，暗黄
#define YELLOW "\033[1;33m"    // 鲜黄
#define BLUE "\033[0;34m"      // 深蓝，暗蓝
#define L_BLUE "\033[1;34m"    // 亮蓝，偏白灰
#define PURPLE "\033[0;35m"    // 深粉，暗粉，偏暗紫
#define L_PURPLE "\033[1;35m"  // 亮粉，偏白灰
#define CYAN "\033[0;36m"      // 暗青色
#define L_CYAN "\033[1;36m"    // 鲜亮青色
#define GRAY "\033[0;37m"      // 灰色
#define WHITE "\033[1;37m"     // 白色，字体粗一点，比正常大，比bold小
#define BOLD "\033[1m"         // 白色，粗体
#define UNDERLINE "\033[4m"    // 下划线，白色，正常大小
#define BLINK "\033[5m"        // 闪烁，白色，正常大小
#define REVERSE "\033[7m"      // 反转，即字体背景为白色，字体为黑色
#define HIDE "\033[8m"         // 隐藏
#define CLEAR "\033[2J"        // 清除
#define CLRLINE "\r\033[K"     // 清除行
#endif

#if defined(WIN32) || defined(WIN64)
static const char* seperator = "\\";
#else
static const char* seperator = "/";
#endif

static const char levels[LEVEL_COUNT][6] = {"trace", "debug", "info ", "warn ", "error", "fatal"};
static LogOption log_option;
static FILE* fp = NULL;
static time_t day = 0;  // 天数据，每天一个日志文件
static char fileName[FILE_LENGTH];

// pthread_t tid;
pthread_mutex_t mut;

static void wirte_console(LogLevel& level, char* message);
static void write_file(LogLevel& level, char* message);
static void generate_file();
static int string_sub(char* dest, char* src, int startIndex, int endIndex);
static int string_indexOf(char* str, char c);

void logger(LogLevel level, const char* function, const char* file, int line, const char* msgfmt, ...) {
  if (level < log_option.level) {
    return;
  }

  pthread_mutex_lock(&mut);  // 加锁，用于对共享变量操作

  /* timestamp */
  char time[25];
  date_millsecond(time, 25);

  char message[MAX_DEBUG];
  int n = snprintf(message, MAX_DEBUG, "%s [%s] [%4lu] %s:%d %s : ", time, levels[level], pthread_self(), file, line, function);
  int sz = n > 0 ? n : 0;
  char* pos = message;

  // 参数处理
  va_list ap;
  va_start(ap, msgfmt);
  vsnprintf(pos + sz, MAX_DEBUG - sz, msgfmt, ap);
  va_end(ap);

  // stdout
  wirte_console(level, message);
  write_file(level, message);

  pthread_mutex_unlock(&mut);  // 解锁
}

#if defined(_WIN32) || defined(_WIN64)
void wirte_console(LogLevel& level, char* message) {
  HANDLE outPutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

  if (level == log_trace) {
    SetConsoleTextAttribute(outPutHandle, gray);
  } else if (level == log_debug) {
    SetConsoleTextAttribute(outPutHandle, white);
  } else if (level == log_info) {
    SetConsoleTextAttribute(outPutHandle, green);
  } else if (level == log_warn) {
    SetConsoleTextAttribute(outPutHandle, yellow);
  } else if (level == log_error) {
    SetConsoleTextAttribute(outPutHandle, red);
  } else if (level == log_fatal) {
    SetConsoleTextAttribute(outPutHandle, purple);
  } else {
    SetConsoleTextAttribute(outPutHandle, white);
  }

  SetConsoleTextAttribute(outPutHandle, white);

  printf("%s\n", message);
  fflush(stdout);
}
#else
void wirte_console(LogLevel& level, char* message) {
  char* color = NULL;
  if (level == log_trace) {
    color = GRAY;
  } else if (level == log_debug) {
    color = CYAN;
  } else if (level == log_info) {
    color = L_GREEN;
  } else if (level == log_warn) {
    color = YELLOW;
  } else if (level == log_error) {
    color = L_RED;
  } else if (level == log_fatal) {
    color = L_PURPLE;
  } else {
    color = NONE;
  }

  if (log_option.is_color) {
    printf("%s%s\n", color, message);
  } else {
    printf("%s\n", message);
  }

  printf(NONE);
  fflush(stdout);
}
#endif

void write_file(LogLevel& level, char* message) {
  if (!log_option.initalized) {
    printf("log is not initilaized\n");
    return;
  }
  generate_file();

  fp = fopen(fileName, "a+");
  if (fp == NULL) {
    char* err_msg = strerror(errno);
    printf("%s(%s:%d) fail to open log file : %s , error code %d , %s \n", __FUNCTION__, __FILE__, __LINE__, fileName, errno, err_msg);
    return;
  }
  // else{
  //   printf("write file %s\n",fileName);
  // }

  fprintf(fp, "%s\n", message);
  fflush(fp);
  fclose(fp);
  fp = NULL;
}

void generate_file() {
  time_t t = time(NULL);
  t += 8 * 60 * 60;
  t /= 24 * 60 * 60;

  if (day != t) {
    memset(fileName, 0, FILE_LENGTH);

    char day_string[12];
    date_day(day_string, 12);

    sprintf(fileName, "%s%s%s-%s.log", log_option.path, seperator, log_option.name, day_string);

    day = t;

    // 删除x天之前文件
    date_day(day_string, 12, log_option.days);
    char del_file[FILE_LENGTH];
    memset(del_file, 0, FILE_LENGTH);
    sprintf(del_file, "%s%s%s-%s.log", log_option.path, seperator, log_option.name, day_string);
    if (access(del_file, F_OK) == 0) {
      int ret = unlink(del_file);
      if (ret != 0) {
        printf("can't delete %s, errno %d,errmsg : %s", del_file, errno, strerror(errno));
      }
    }
  }
}

static pthread_t p;
static void rm_history_task() {
  struct timeb tmp;
  ftime(&tmp);
  tmp.time -= 60 * 60 * 24 * 10;  // 10天前的时间戳
  tmp.time += 8 * 60 * 60;        // 时区
  struct tm* gmt = gmtime(&tmp.time);
  int days = (gmt->tm_year + 1900) * 10000 + (gmt->tm_mon + 1) * 100 + gmt->tm_mday;

  DIR* dp = opendir(log_option.path);
  if (dp == NULL) {
    printf("can't open dir %s\n",log_option.path);
    return;
  }
  struct dirent* entry;
  while ((entry = readdir(dp)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0) {
      continue;
    }
    if (strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    char* fileName = entry->d_name;
    int index = string_indexOf(fileName, '-');
    if (index < 0) {
      // logger_error("can't parse date time from file %s", fileName);
      continue;
    }
    fileName += index;
    fileName++;

    char buffer[5];

    memset(buffer, 0, 5);
    string_sub(buffer, fileName, 0, 3);
    int year = atoi(buffer);

    memset(buffer, 0, 5);
    string_sub(buffer, fileName, 5, 6);
    int month = atoi(buffer);

    memset(buffer, 0, 5);
    string_sub(buffer, fileName, 8, 9);
    int day = atoi(buffer);

    int fileDay = year * 10000 + month * 100 + day;
    if (fileDay < days) {
      logger_info("remove log history file %s", entry->d_name);

      char fullName[512];
      memset(fullName, 0, 512);
      strcat(fullName, log_option.path);
      strcat(fullName, "/");
      strcat(fullName, entry->d_name);
      int rc = unlink(fullName);
      if (rc != 0) {
        logger_error("can't delete file %s", fullName);
      }
    }
  }
  closedir(dp);
  dp = NULL;
}

static void* rm_history_job(void* arg) {
  while (true) {
    // sleep(strlen(log_option.name) * 60);
    usleep(strlen(log_option.name) * 60) * 1000000;

    rm_history_task();
  }
}

/* 日志初始化 */
int logger_initialize(LogOption option) {
  pthread_mutex_init(&mut, NULL);

  // 初始化日志选项
  log_option = option;

  // 创建日志目录
  char path[512];
  strcpy(path, option.path);
  mkdirs(path);

  // 启动日志监控线程
  pthread_create(&p, NULL, rm_history_job, NULL);

  log_option.initalized = true;
  //  printf("option.initalized = true \n");

  return 0;  // fp == NULL ? -1 : 0;
}

char logger_parseLevel(const char* level) {
  for (int i = 0; i < LEVEL_COUNT; i++) {
    int count = 0;
    for (int j = 0; j < 4; j++) {
      if (levels[i][j] != level[j]) {
        break;
      } else {
        count++;
      }
    }

    if (count == 4) {
      // printf("parsed level %s\n", level);
      return i;
    }
  }

  printf("can't parse log level\n");

  return -1;
}

//////////////////////////////////////////////////////////////////////

int mkdirs(char* path) {
  printf("log path: %s \n", path);

  char newPath[1024];
  memset(newPath, 0, 1024);

#if defined(_WIN32) || defined(_WIN64)
#else
  if (path[0] == seperator[0]) {
    strcat(newPath, seperator);
  }
#endif

  char* p = strtok(path, seperator);

  while (p) {
    strcat(newPath, p);
    strcat(newPath, seperator);

    if (access(newPath, F_OK) != F_OK) {
      // printf("mkdir %s\n", newPath);

#if defined(_WIN32) || defined(_WIN64)
      int ret = mkdir(newPath);
      if (ret != 0) {
        printf("cant't mkdir %s \n", newPath);
        return 1;
      }
#else
      // int ret = mkdir(newPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      int ret = mkdir(newPath, 0777);
      // https://blog.csdn.net/yinjian1013/article/details/78611009

      if (ret < 0) {
        printf("cant't mkdir %s \n", newPath);
        return 1;
      }
#endif
    } else {
      // printf("%s is exsist\n", newPath);
    }

    p = strtok(NULL, seperator);

    // printf("-- %s\n", p);
  }

  return 0;
}

void date_day(char* date, int len) {
  time_t t;
  time(&t);
  t += 8 * 60 * 60;  // 北京时区加上8小时
  struct tm* gmt = gmtime(&t);

  memset(date, 0, len);
  strftime(date, len, "%Y-%m-%d", gmt);
}

void date_day(char* date, int len, int days) {
  time_t t;
  time(&t);
  t += 8 * 60 * 60;  // 北京时区加上8小时
  t -= days * 24 * 60 * 60;
  struct tm* gmt = gmtime(&t);

  memset(date, 0, len);
  strftime(date, len, "%Y-%m-%d", gmt);
}

void date_second(char* date, int len) {
  time_t t;
  time(&t);
  t += 8 * 60 * 60;  // 北京时区加上8小时
  struct tm* gmt = gmtime(&t);

  memset(date, 0, len);
  strftime(date, len, "%Y-%m-%d %H:%M:%S", gmt);
}

void date_millsecond(char* date, int len) {
  const int length = 20;
  char str[length];
  memset(str, 0, length);

  struct timeb time = {0, 0};
  ftime(&time);
  time.time += 8 * 60 * 60;  // 北京时区加上8小时
  struct tm* gmt = gmtime(&time.time);

  strftime(str, length, "%Y-%m-%d %H:%M:%S", gmt);

  memset(date, 0, len);

  sprintf(date, "%s,%03d", str, time.millitm);
}

int string_indexOf(char* str, char c) {
  int len = strlen(str);
  for (int i = 0; i < len; i++) {
    if (str[i] == c) {
      return i;
    }
  }

  return -1;
}

int string_sub(char* dest, char* src, int startIndex, int endIndex) {
  if (startIndex >= endIndex) {
    return -1;
  }

  if (startIndex < 0) {
    return -1;
  }

  int len = strlen(src);
  if (endIndex >= len) {
    return -1;
  }

  for (int i = startIndex; i <= endIndex; i++) {
    dest[i - startIndex] = src[i];
  }

  return 0;
}