#include "ToReduce.hpp"

#include "core/DscConfig.hpp"
#include "core/cjson.hpp"
#include "core/log.hpp"
#include "lwdee/lwdee.h"

void ToReduce::create_dcos() {
  // int splits = DscConfig::instance()->splitNums2;
  // for (int i = 0; i < splits; i++) {
  //   DCO dco = lwdee::create_dco("ReduceDCO");
  //   reduceDcos.push_back(dco);
  // }

  // releaseThread = std::thread(&ToReduce::releaseDdo, this);
}

void ToReduce::send(vector<string>& words) {
  int split = DscConfig::instance()->splitNums2;

  vector<vector<string>> reduceWords(split);
  for (int i = 0; i < words.size(); i++) {
    string& word = words[i];
    int x = _hash(word) % split;
    reduceWords[x].push_back(word);
  }

  for (int i = 0; i < split; i++) {
    DCO dco = lwdee::create_dco_byindex(i, "ReduceDCO");
    auto str = json(reduceWords[i]);

    logger_debug("invoke reduce dco");
    usleep(1000000 / 100);
    auto ddoId = dco.async("reduce", str);
    ddoIds.push(std::make_pair(ddoId, dco));
  }
}

string ToReduce::json(vector<string>& words) {
  cJSON* nodes = cJSON_CreateArray();
  for (string& word : words) {
    auto item = cJSON_CreateString(word.c_str());
    cJSON_AddItemToArray(nodes, item);
  }
  string jsonText = cJSON_Print(nodes);

  return jsonText;
}

void ToReduce::releaseDdo() {
  while (true) {
    if (ddoIds.size() > 0) {
      ddoIds.pop();
      auto i = ddoIds.front();
      i.second.wait(i.first);
      DDO(i.first).releaseGlobal();
    }

    usleep(1000000 / 10);
  }
}