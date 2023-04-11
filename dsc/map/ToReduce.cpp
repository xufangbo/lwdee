#include "ToReduce.hpp"

#include "core/DscConfig.hpp"
#include "core/cjson.hpp"
#include "core/log.hpp"
#include "lwdee/lwdee.h"

void ToReduce::create_dcos(PartitionMap* input) {
  for (auto& reduceVoxorId : input->reduceVoxors) {
    DCO dco = lwdee::get_dco(reduceVoxorId);
    reduceDcos.push_back(dco);
  }

  releaseThread = std::thread(&ToReduce::releaseDdo, this);
  releaseThread.detach();
}

void ToReduce::send(vector<string>& words) {
  int split = reduceDcos.size();

  vector<vector<string>> reduceWords(split);
  for (int i = 0; i < words.size(); i++) {
    string& word = words[i];
    int x = _hash(word) % split;
    reduceWords[x].push_back(word);
  }

  for (int i = 0; i < split; i++) {
    DCO* dco = reduceDcos.data() + i;

    auto str = json(reduceWords[i]);

    logger_debug("invoke reduce dco");

    auto ddoId = dco->async("reduce", str);

    // logger_warn("ready to lock");
    // mut.lock();
    ddoIds.push_back(std::make_pair(ddoId, dco));
    // mut.unlock();
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
    
    // logger_info("remove wait reduce ddo , %d", ddoIds.size());
    int size = ddoIds.size();
    for (int i = 0; i < size - 1; i++) {
      if (!ddoIds.empty()) {
        auto i = ddoIds.front();
        i.second->wait(i.first);
        DDO(i.first).releaseGlobal();
        ddoIds.pop_front();
      }
    }
    
    
    usleep(1000000 / 10);
  }
}