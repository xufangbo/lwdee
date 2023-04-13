#include "Reducer.hpp"
#include <algorithm>
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

void Reducer::accept(std::vector<ReduceRecord>* records, PartitionReduce* input) {
  this->input = input;
  mut.lock();

  uint64_t now = Stopwatch::currentMilliSeconds();
  auto newWindowTs = this->getCurrentWindow();

  uint64_t b_sum = 0;
  uint64_t b_size = records->size();

  for (auto& record : *records) {
    this->records->push_back(record);
    b_sum += (now - (uint64_t)record.ts);
    // logger_info("%lldms = %lldms - %lldms", (now - (uint64_t)record.ts), now, (uint64_t)record.ts);
  }

  if (records->empty()) {
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
    this->reduce();

    uint64_t t_sum = this->sum;
    uint64_t t_size = this->count;
    logger_trace("reduce total delay (%lldms / %lld = %.3lfs)", t_sum, t_size, (t_sum / t_size) * 1.0 / 1000);
  }

  mut.unlock();
  // logger_debug("> accept");
}

void Reducer::reduce() {
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

  logger_debug("reduce(partiton index %d) %lld -> %lld", input->index, records->size(), map->size());

  int index = 0;
  for (auto& it : *map) {
    if (++index > 10) {
      break;
    }
    printf("(%s,%d) ", it.first.c_str(), it.second);
  }
  if (map->size() > 10) {
    printf("  ...\n");
  } else {
    printf("\n");
  }

  map.reset();
  records->clear();

  // logger_trace("> reduce");
}

uint64_t Reducer::getCurrentWindow() {
  uint64_t now = Stopwatch::currentMilliSeconds();
  return now + window - (now % window);
}