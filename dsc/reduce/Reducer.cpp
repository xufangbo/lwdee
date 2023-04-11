#include "Reducer.hpp"
#include <algorithm>
#include "core/DscConfig.hpp"

Reducer::Reducer() {
  DscConfig* conf = DscConfig::instance();
  maps.resize(conf->splitNums1);
}

void Reducer::accept(int mapIndex, Words& words) {
  mut.lock();
  MapQueue* mapQueue = maps.data() + mapIndex;
  mapQueue->push_back(words);

  if (isFull()) {
    this->reduce();
  }
  mut.unlock();
}

void Reducer::reduce() {
  std::map<std::string, int> map;
  for (MapQueue& queue : maps) {
    Words& words = queue.front();

    for (std::string& i : words) {
      auto it = map.find(i);
      if (it == map.end()) {
        auto pair = std::pair<std::string, int>(i, 1);
        map.insert(pair);
      } else {
        it->second++;
      }
    }
    queue.pop_front();
  }

  // typedef std::pair<std::string, int> MapItem;
  // std::vector<MapItem> items;
  // for (auto& i : map) {
  //   items.push_back(i);
  // }
  // std::sort(items.begin(), items.end(), [](MapItem& l, MapItem& r) { return l.second - r.second; });

  for (auto& item : map) {
    printf("(%s,%d) ", item.first.c_str(), item.second);
  }
  printf("\n");
}

bool Reducer::isFull() {
  for (MapQueue& queue : maps) {
    if (queue.empty()) {
      return false;
    }
  }

  return true;
}
