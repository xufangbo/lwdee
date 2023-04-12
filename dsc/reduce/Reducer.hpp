#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <vector>
#include "core/Partition.h"

typedef std::vector<DeviceRecord> Records;

class Reducer {
 private:
  int window = 0;  // 秒为单位
  time_t currentTs = 0;
  std::shared_ptr<Records> records = std::make_shared<Records>();

 private:
  void reduce();
  std::mutex mut;

 public:
  Reducer();
  void accept(std::vector<DeviceRecord>* records);
};