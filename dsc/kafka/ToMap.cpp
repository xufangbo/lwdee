#include "ToMap.h"

#include "core/DscConfig.hpp"
#include "core/Stopwatch.h"
#include "core/cjson.hpp"
#include "core/log.hpp"
#include "lwdee/lwdee.h"
#include "map/MapDCO.h"

ToMap::ToMap() {
  // releaseThread = std::thread(&ToMap::releaseDdo, this);
}

void ToMap::create_dco(int size) {
  // for (int i = 0; i < size; i++) {
  //   DCO dco = lwdee::create_dco("MapDCO");
  //   mapDocs.push_back(dco);

  //   vector<string> lines;
  //   mapLines.push_back(lines);
  // }
  auto dsconf = DscConfig::instance();
  for (int i = 0; i < dsconf->splitNums1; i++) {
    vector<string> lines;
    mapLines.push_back(lines);
  }
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
  // auto dco = this->mapDocs.data() + index;
  auto dco = lwdee::create_dco("MapDCO");
  auto lines = this->mapLines.data() + index;

  string jsonText = json(lines);
  lines->clear();

  logger_trace("invoke map dco");
  DDOId ddoId = dco.async("map", jsonText);

  ddoIds.push(std::make_pair(ddoId, dco));
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
    if (ddoIds.size() > 0) {
      ddoIds.pop();
      auto i = ddoIds.front();
      i.second.wait(i.first);
      DDO(i.first).releaseGlobal();
    }

    usleep(1000000 / 100);
  }
}