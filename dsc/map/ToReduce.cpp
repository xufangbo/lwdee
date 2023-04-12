#include "ToReduce.hpp"

#include "core/DscConfig.hpp"
#include "core/cjson.hpp"
#include "core/log.hpp"
#include "lwdee/lwdee.h"

void ToReduce::create_dcos(PartitionMap* input) {
  this->input = input;

  for (auto& reduceVoxorId : input->reduceVoxors) {
    DCO dco = lwdee::get_dco(reduceVoxorId);
    reduceDcos.push_back(dco);
  }

  releaseThread = std::thread(&ToReduce::releaseDdo, this);
  releaseThread.detach();
}

void ToReduce::send(vector<DeviceRecord>* words) {
  int split = reduceDcos.size();

  auto reduceWords = std::make_shared<vector<vector<DeviceRecord>>>(split);
  for (int i = 0; i < words->size(); i++) {
    DeviceRecord& word = words->at(i);
    int x = _hash(word.did) % split;
    reduceWords->at(x).push_back(word);
  }

  for (int i = 0; i < split; i++) {
    DCO* dco = reduceDcos.data() + i;

    ReduceData reduceData(input->index, &reduceWords->at(i));

    auto jsonText = reduceData.toJson();

    auto ddoId = dco->async("reduce", jsonText);

    ddoIds->push_back(std::make_pair(ddoId, dco));
  }
}

void ToReduce::releaseDdo() {
  while (true) {
    // logger_info("remove wait reduce ddo , %d", ddoIds->size());
    int size = ddoIds->size();
    for (int i = 0; i < size - 1; i++) {
      if (!ddoIds->empty()) {
        auto i = ddoIds->front();
        i.second->wait(i.first);
        DDO(i.first).releaseGlobal();
        ddoIds->pop_front();
      }
    }

    usleep(1000000 / 10);
  }
}