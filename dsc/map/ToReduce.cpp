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

  for (string& word : words) {
    int i = _hash(word) % split;
    reduceWords[i] += "|";
    reduceWords[i] += word;
  }

  for (int i = 0; i < split; i++) {
    auto& str = reduceWords[i];
    if (str.size() > 0) {
      str[str.size() - 1] = '\0';
    }
  }

  for (int i = 0; i < split; i++) {
    auto dco = reduceDcos[i];
    auto& str = reduceWords[i];

    auto ddoId = dco.async("accept", str);

    ddoIds.push(std::make_pair(ddoId, &dco));
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