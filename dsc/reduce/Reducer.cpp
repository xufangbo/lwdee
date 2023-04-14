#include "Reducer.hpp"
#include <algorithm>
#include <fstream>
#include <memory>
#include "core/DscConfig.hpp"
#include "core/Stopwatch.h"
#include "core/log.hpp"

Reducer::Reducer() {
  auto conf = DscConfig::instance();
  this->window = conf->window * 1000;
  this->currentWindowTs = this->getCurrentWindow();

  this->sum = 0;
  this->count = 0;
}

void writeFile(int index, uint64_t c_input, uint64_t c_output, double b_delay, uint64_t b_sum, uint64_t b_size, double t_delay, uint64_t t_sum, uint64_t t_size) {
  auto conf = DscConfig::instance();
  std::string fileName = conf->outputFile + conf->nodeName + "_reduce_" + std::to_string(index) + ".csv";

  std::ofstream f(fileName, std::ios_base::app);
  if (!f.is_open()) {
    logger_trace("can't open file %s", fileName.c_str());
    return;
  }

  char time[25];
  date_millsecond(time, 25);
  time[19] = ' ';

  f << time << ",";

  f << index << ",";
  f << c_input << ",";
  f << c_output << ",";
  f << std::fixed << setprecision(3) << b_delay << ",";
  f << b_sum << ",";
  f << b_size << ",";
  f << std::fixed << setprecision(3) << t_delay << ",";
  f << t_sum << ",";
  f << t_size << ",";

  f << std::endl;

  f.flush();
  f.close();
}

void Reducer::accept(std::vector<ReduceRecord>* b_records, PartitionReduce* input) {
  this->input = input;
  mut.lock();

  uint64_t now = Stopwatch::currentMilliSeconds();
  auto newWindowTs = this->getCurrentWindow();

  uint64_t b_sum = 0;
  uint64_t b_size = b_records->size();

  for (auto& record : *b_records) {
    this->records->push_back(record);
    b_sum += (now - (uint64_t)record.ts);
    // logger_info("%lldms = %lldms - %lldms", (now - (uint64_t)record.ts), now, (uint64_t)record.ts);
  }

  if (b_records->empty()) {
    // logger_info("accept reduce, no data");
  } else {
    // uint64_t o_sum = this->sum;
    // uint64_t o_size = this->count;

    this->sum = this->sum + b_sum;
    this->count = this->count + b_size;

    // uint64_t t_sum = this->sum;
    // uint64_t t_size = this->count;

    // logger_info("sum  : %lld = %lld + %lld", t_sum, o_sum, b_sum);
    // logger_info("size : %lld = %lld + %lld", t_size, o_size, b_size);

    // logger_info("accept reduce, this delay(%lldms / %lld = %.3lfs),total(%lldms / %lld = %.3lfs)",  b_sum, b_size, ((int64_t)b_sum) * 1.0 / b_size / 1000, t_sum, t_size, (t_sum / t_size) * 1.0 / 1000);
  }

  if (newWindowTs > currentWindowTs) {
    currentWindowTs = newWindowTs;
    int reduceSize = this->reduce();

    uint64_t t_sum = this->sum;
    uint64_t t_size = this->count;

    if (b_size > 0 &&  t_size > 0) {
      
      auto b_delay = ((int64_t)b_sum) * 1.0 / b_size / 1000;
      auto t_delay = (t_sum / t_size) * 1.0 / 1000;

      logger_warn("reduce ,(partiton index %d) %lld -> %lld, total delay (%lldms / %lld = %.3lfs)", input->index, this->records->size(), reduceSize, t_sum, t_size, t_delay);

      writeFile(input->index, this->records->size(), reduceSize, b_delay, b_sum, b_size, t_delay, t_sum, t_size);
    }

    this->records->clear();
  }

  mut.unlock();
  // logger_debug("> accept");
}

int Reducer::reduce() {
  // logger_trace("< reduce");

  typedef std::pair<std::string, int> Pair;
  typedef std::map<std::string, int> Map;

  auto map = std::make_shared<Map>();

  for (auto& record : *records) {
    auto it = map->find(record.did);
    if (it == map->end()) {
      auto pair = Pair(record.did, 1);
      map->insert(pair);
    } else {
      it->second++;
    }
  }

  // printf("\n");

  // int index = 0;
  // for (auto& it : *map) {
  //   if (++index > 10) {
  //     break;
  //   }
  //   printf("(%s,%d) ", it.first.c_str(), it.second);
  // }
  // if (map->size() > 10) {
  //   printf("  ...\n");
  // } else {
  //   printf("\n\n");
  // }

  int reduceSize = map->size();
  map.reset();

  // logger_trace("> reduce");

  return reduceSize;
}

uint64_t Reducer::getCurrentWindow() {
  uint64_t now = Stopwatch::currentMilliSeconds();
  return now + window - (now % window);
}