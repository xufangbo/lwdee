#include <atomic>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <vector>
#include "core/Partition.h"

typedef std::vector<ReduceRecord> Records;

class Reducer {
 private:
  // std::atomic<uint64_t> sum;
  // std::atomic<uint64_t> count;
  uint64_t sum;
  uint64_t count;

 private:
  PartitionReduce *input;
  uint64_t window = 0;  // 秒为单位
  uint64_t currentWindowTs = 0;
  uint64_t getCurrentWindow();
  std::shared_ptr<Records> records = std::make_shared<Records>();

 private:
  int reduce();
  std::mutex mut;

 public:
  Reducer();
  void accept(std::vector<ReduceRecord>* b_records,PartitionReduce *input);
};