#include "Reducer.hpp"
#include <algorithm>
#include <memory>
#include "core/DscConfig.hpp"
#include "core/Stopwatch.h"
#include "core/log.hpp"

Reducer::Reducer() {
  auto conf = DscConfig::instance();
  this->window = conf->window;

  time_t now = Stopwatch::currentTs();
  currentTs = now + window - (now % window);
}

void Reducer::accept(std::vector<DeviceRecord>* inputs) {
  int now = Stopwatch::currentTs();
  auto nowTs = now + window - (now % window);

  mut.lock();

  for (auto& record : *inputs) {
    this->records->push_back(record);
  }

  if (nowTs > currentTs) {
    currentTs = nowTs;
    this->reduce();
  }

  inputs->clear();

  mut.unlock();
  // logger_debug("> accept");
}

void Reducer::reduce() {
  // logger_trace("< reduce");

  time_t now = Stopwatch::currentTs();
  uint64_t sum = 0;
  for (auto& record : *records) {
    // if (now - record.ts > 100) {
    //   std::cout << now << " - " << record.ts << " = " <<  now - record.ts << std::endl;
    // }
    sum += (now - record.ts);
  }
  logger_info("reduce %ld records,avrage delay %fs", records->size(), sum * 1.0 / records->size());

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

  logger_trace("> reduce");
}