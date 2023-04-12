#include "Reducer.hpp"
#include <algorithm>
#include "core/DscConfig.hpp"
#include "core/log.hpp"
#include <memory>

Reducer::Reducer() {
  auto conf = DscConfig::instance();
  this->window = conf->window;

  time_t ts = time(NULL);
  currentTs = ts + window - (ts % window);
}

void Reducer::accept(int mapIndex, std::shared_ptr<std::vector<DeviceRecord>> inputs) {
  time_t ts = time(NULL);
  auto nowTs = ts + window - (ts % window);

  mut.lock();

  for (auto& record : *inputs) {
    this->records->push_back(record);
  }

  if (nowTs > currentTs) {
    currentTs = nowTs;
    this->reduce();
  }

  inputs->clear();
  inputs.reset();

  mut.unlock();
  // logger_debug("> accept");
}

void Reducer::reduce() {
  // logger_trace("< reduce");

  time_t now = time(NULL);
  time_t sum = 0;
  for (auto& record : *records) {
    sum += (now - record.ts);
  }
  logger_info("reduce %ld records,avrage delay %f",records->size(), sum * 1.0 / records->size());

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

  for(auto &it : *map){
    printf("(%s,%d) ",it.first.c_str(),it.second);
  }
  printf("\n");

  map.reset();
  records->clear();

  logger_trace("> reduce");
}