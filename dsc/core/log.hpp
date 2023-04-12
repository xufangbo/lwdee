#pragma once

#define logger_trace(fromat, ...) logger(log_trace, __FUNCTION__, __FILE__, __LINE__, fromat, ##__VA_ARGS__)
#define logger_debug(fromat, ...) logger(log_debug, __FUNCTION__, __FILE__, __LINE__, fromat, ##__VA_ARGS__)
#define logger_info(fromat, ...) logger(log_info, __FUNCTION__, __FILE__, __LINE__, fromat, ##__VA_ARGS__)
#define logger_warn(fromat, ...) logger(log_warn, __FUNCTION__, __FILE__, __LINE__, fromat, ##__VA_ARGS__)
#define logger_error(fromat, ...) logger(log_error, __FUNCTION__, __FILE__, __LINE__, fromat, ##__VA_ARGS__)
#define logger_fatal(fromat, ...) logger(log_fatal, __FUNCTION__, __FILE__, __LINE__, fromat, ##__VA_ARGS__)

typedef enum
{
    log_trace = 0,
    log_debug = 1,
    log_info = 2,
    log_warn = 3,
    log_error = 4,
    log_fatal = 5
} LogLevel;

typedef struct
{
    /**
     * 选项是否已经初始化
     * **/
    bool initalized;
    /**
     * 日志级别
     * **/
    LogLevel level;

    /**
     * 日志目录
     */
    char path[512];

    /**
     * 名称
     */
    char name[30];

    /**
     * 启用linux控制台颜色
     * 在cmd控制台中linux的颜色不能正常显示
     */
    int is_color;

    bool writeFile;

    /**
     * 日志保留天数
     */
    int days = 10;
} LogOption;

/* 执行日志 */
void logger(LogLevel level, const char *function, const char *file, int line, const char *fromat, ...);

/* 设置日志配置 */
int logger_initialize(LogOption option);

/* 根据字符解析为日志类型枚举 */
char logger_parseLevel(const char *level);

////////////////////////////////////////
int mkdirs(char *path);
void date_day(char *date, int len);
void date_day(char *date, int len, int days);
void date_second(char *date, int len);
void date_millsecond(char *date, int len);