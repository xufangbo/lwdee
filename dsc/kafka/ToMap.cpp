#include "ToMap.h"

#include "core/DscConfig.hpp"
#include "core/Stopwatch.h"
#include "core/cjson.hpp"
#include "core/log.hpp"
#include "lwdee/lwdee.h"
#include "map/MapDCO.h"

ToMap::ToMap() {
}

ToMap::~ToMap() {
  if (mapLines != nullptr) {
    delete mapLines;
    mapLines = nullptr;
  }

  if (ddoIds != nullptr) {
    delete ddoIds;
    ddoIds = nullptr;
  }
}

void ToMap::create_dco(PartitionKafka* input) {
  this->input = input;
  this->window = input->window;

  for (auto& mapVoxorId : input->mapVoxors) {
    DCO dco = lwdee::get_dco(mapVoxorId);
    mapDocs.push_back(dco);

    vector<string> lines;
    mapLines->push_back(lines);
  }

  releaseThread = std::thread(&ToMap::releaseDdo, this);
  releaseThread.detach();
}

void ToMap::accept(RdKafka::Message* message) {
  // logger_trace("offset: %d,%s",message->offset(), static_cast<const char*>(message->payload()));

  string line = string(message->len() + 1, '\0');
  memcpy((void*)line.data(), message->payload(), message->len());

  int index = this->nextMap();
  auto lines = this->mapLines->data() + index;
  lines->push_back(line);

  if (lines->size() >= window) {
    this->toMap(index);
  }
};

int ToMap::nextMap() {
  currentMap++;
  if (currentMap >= mapLines->size()) {
    currentMap = 0;
  }
  return currentMap;
}

void ToMap::toMap(int index) {
  auto dco = this->mapDocs.data() + index;
  auto lines = this->mapLines->data() + index;

  auto jsonText = StringsSerializer::toJson(input->index, lines);
  lines->clear();

  DDOId ddoId = dco->async("map", jsonText);

  ddoIds->push_back(std::make_pair(ddoId, dco));
}

void ToMap::releaseDdo() {
  while (true) {
    // logger_debug("remove wait map ddo , %d", ddoIds->size());
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