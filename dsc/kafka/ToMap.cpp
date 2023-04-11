#include "ToMap.h"

#include "core/DscConfig.hpp"
#include "core/Stopwatch.h"
#include "core/cjson.hpp"
#include "core/log.hpp"
#include "lwdee/lwdee.h"
#include "map/MapDCO.h"

ToMap::ToMap() {
}

void ToMap::create_dco(std::shared_ptr<PartitionKafka> input) {
  for (auto& mapVoxorId : input->mapVoxors) {
    DCO dco = lwdee::get_dco(mapVoxorId);
    mapDocs.push_back(dco);

    vector<string> lines;
    mapLines.push_back(lines);
  }

  releaseThread = std::thread(&ToMap::releaseDdo, this);
  releaseThread.detach();
}

void ToMap::accept(RdKafka::Message* message) {
  // logger_trace("offset: %d,%s",message->offset(), static_cast<const char*>(message->payload()));

  string line = string(message->len() + 1, '\0');
  memcpy((void*)line.data(), message->payload(), message->len());

  int index = this->nextMap();
  auto lines = this->mapLines.data() + index;
  lines->push_back(line);

  if (lines->size() >= window) {
    this->toMap(index);
  }
};

int ToMap::nextMap() {
  currentMap++;
  if (currentMap >= mapLines.size()) {
    currentMap = 0;
  }
  return currentMap;
}

void ToMap::toMap(int index) {
  auto dco = this->mapDocs.data() + index;
  auto lines = this->mapLines.data() + index;

  string jsonText = json(lines);
  lines->clear();

  logger_trace("invoke map dco");
  DDOId ddoId = dco->async("map", jsonText);

  // logger_warn("ready to lock");
  // mut.lock();
  ddoIds.push_back(std::make_pair(ddoId, dco));
  // mut.unlock();
}

string ToMap::json(vector<string>* lines) {
  cJSON* nodes = cJSON_CreateArray();
  for (string& line : *lines) {
    auto item = cJSON_CreateString(line.c_str());
    cJSON_AddItemToArray(nodes, item);
  }
  string jsonText = cJSON_Print(nodes);

  return jsonText;
}

void ToMap::releaseDdo() {
  while (true) {
    
    // logger_debug("remove wait map ddo , %d", ddoIds.size());
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