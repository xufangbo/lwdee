#include "Reducer.hpp"
#include <algorithm>
#include "core/DscConfig.hpp"

Reducer::Reducer() {
  DscConfig* conf = DscConfig::instance();
  maps.resize(conf->splitNums1);
}

void Reducer::accept(int mapIndex, std::shared_ptr<Words> words) {
  mut.lock();

  MapQueue* mapQueue = maps.data() + mapIndex;
  mapQueue->push_back(words);

  if (isFull()) {
    this->reduce();
  }

  mut.unlock();
}

void Reducer::reduce() {

  typedef std::pair<std::string,int> WordPair;
  typedef std::vector<WordPair> ReduceWords;

  std::shared_ptr<ReduceWords> all_words= std::make_shared<ReduceWords>();

  for (MapQueue& queue : maps) {
    std::shared_ptr<Words> words = queue.front();

    for (std::string& i : *words) {
      auto it = std::find_if(all_words->begin(),all_words->end(),[&i](WordPair &t){return t.first ==i; });
      if (it == all_words->end()) {
        auto pair = WordPair(i, 1);
        all_words->push_back(pair);
      } else {
        it->second++;
      }
    }
    queue.pop_front();
  }

  std::sort(all_words->begin(), all_words->end(), [](WordPair& l, WordPair& r) { return l.second > r.second; });
  for (auto &i : *all_words) {
    printf("(%s,%d) ", i.first.c_str(), i.second);
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
