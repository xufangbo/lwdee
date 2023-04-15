#include <sys/time.h>

#include <atomic>
#include <iostream>

template <typename... Args>
std::string string_format(const std::string& format, Args... args) {
  size_t size = 1 + snprintf(nullptr, 0, format.c_str(), args...);
  char bytes[size];
  snprintf(bytes, size, format.c_str(), args...);
  return std::string(bytes);
}

class SnowflakeIdGenertor {
 public:
  /**
   * 构造函数
   *
   * @param workerId     工作ID (0~31)
   * @param datacenterId 数据中心ID (0~31)
   */
  SnowflakeIdGenertor(long workerId, long datacenterId) {
    if (workerId > maxWorkerId || workerId < 0) {
      std::cout << (string_format(
          "worker Id can't be greater than %d or less than 0", maxWorkerId));
      return;
    }
    if (datacenterId > maxDatacenterId || datacenterId < 0) {
      std::cout << (string_format(
          "datacenter Id can't be greater than %d or less than 0",
          maxDatacenterId));
      return;
    }
    this->workerId = workerId;
    this->datacenterId = datacenterId;
  }

  /**
   * 获得下一个ID (该方法是线程安全的)
   *
   * @return SnowflakeId
   */
  long long nextId() {
    long long timestamp = timeGen();

    // 如果当前时间小于上一次ID生成的时间戳，说明系统时钟回退过这个时候应当抛出异常
    if (timestamp < lastTimestamp) {
      std::cout << (string_format(
          "Clock moved backwards.  Refusing to generate id for %d milliseconds",
          lastTimestamp - timestamp));
      return 0;
    }

    // 如果是同一时间生成的，则进行毫秒内序列
    if (lastTimestamp == timestamp) {
      sequence = (sequence + 1) & sequenceMask;
      // 毫秒内序列溢出
      if (sequence == 0) {
        // 阻塞到下一个毫秒,获得新的时间戳
        timestamp = tilNextMillis(lastTimestamp);
      }
    }
    // 时间戳改变，毫秒内序列重置
    else {
      sequence = 0L;
    }

    // 上次生成ID的时间截
    lastTimestamp = timestamp;

    // 移位并通过或运算拼到一起组成64位的ID
    return ((timestamp - twepoch) << timestampLeftShift)  //
           | (datacenterId << datacenterIdShift)          //
           | (workerId << workerIdShift)                  //
           | sequence;
  }

  /**
   * 阻塞到下一个毫秒，直到获得新的时间戳
   *
   * @param lastTimestamp 上次生成ID的时间截
   * @return 当前时间戳
   */
  long long tilNextMillis(long long lastTimestamp) {
    long long timestamp = timeGen();
    while (timestamp <= lastTimestamp) {
      timestamp = timeGen();
    }
    return timestamp;
  }

  /**
   * 返回以毫秒为单位的当前时间
   *
   * @return 当前时间(毫秒)
   */
  long long timeGen() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long nowTimeMs = (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return nowTimeMs;
  }

 private:
  /**
   * 开始时间截 (2020-01-01)
   */
  const long long twepoch = 1577808000000L;

  /**
   * 机器id所占的位数
   */
  const long workerIdBits = 5L;

  /**
   * 数据标识id所占的位数
   */
  const long datacenterIdBits = 5L;

  /**
   * 支持的最大机器id，结果是31
   * (这个移位算法可以很快的计算出几位二进制数所能表示的最大十进制数)
   */
  const long maxWorkerId = -1L ^ (-1L << workerIdBits);

  /**
   * 支持的最大数据标识id，结果是31
   */
  const long maxDatacenterId = -1L ^ (-1L << datacenterIdBits);

  /**
   * 序列在id中占的位数
   */
  const long sequenceBits = 12L;

  /**
   * 机器ID向左移12位
   */
  const long workerIdShift = sequenceBits;

  /**
   * 数据标识id向左移17位(12+5)
   */
  const long datacenterIdShift = sequenceBits + workerIdBits;

  /**
   * 时间截向左移22位(5+5+12)
   */
  const long timestampLeftShift =
      sequenceBits + workerIdBits + datacenterIdBits;

  /**
   * 生成序列的掩码，这里为4095 (0b111111111111=0xfff=4095)
   */
  const long sequenceMask = -1L ^ (-1L << sequenceBits);

  /**
   * 工作机器ID(0~31)
   */
  long workerId;

  /**
   * 数据中心ID(0~31)
   */
  long datacenterId;

  /**
   * 毫秒内序列(0~4095)
   */
  std::atomic<long> sequence = 0L;

  /**
   * 上次生成ID的时间截
   */
  long long lastTimestamp = -1L;
};