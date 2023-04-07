#include "ToReduce.hpp"

#include "core/DscConfig.hpp"
#include "lwdee/lwdee.h"

void ToReduce::create_dcos() {
  int splits = DscConfig::instance()->splitNums2;
  for (int i = 0; i < splits; i++) {
    DCO dco = lwdee::create_dco("ReduceDCO");
    reduceDcos.push_back(dco);
  }

  releaseThread = std::thread(&ToReduce::releaseDdo, this);
}

void ToReduce::send(vector<string>& words) {
  int split = DscConfig::instance()->splitNums2;

  vector<string> reduceWords(split);

  for (int i = 0; i < words.size(); i++) {
    string& word = words[i];
    int x = _hash(word) % split;

    reduceWords[x] += word;
    if (i != (words.size() - 1)) {
      reduceWords[x] += "|";
    }
  }

  for (int i = 0; i < split; i++) {
    auto dco = reduceDcos[i];
    auto& str = reduceWords[i];

    auto ddoId = dco.async("reduce", str);

    // ddoIds.push(std::make_pair(ddoId, &dco));
  }
}

void ToReduce::releaseDdo() {
  while (true) {
    if (ddoIds.size() > 0) {
      ddoIds.pop();
      auto i = ddoIds.front();
      i.second->wait(i.first);
      DDO(i.first).releaseGlobal();
    }

    usleep(1000000 / 100);
  }
}