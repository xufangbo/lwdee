#include "ToMap.h"

#include <time.h>
#include "core/DscConfig.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.h"
#include "core/cjson.hpp"
#include "core/log.hpp"
#include "lwdee/lwdee.h"
#include "map/MapDCO.h"
#include "matrix/LinuxMatrix.h"

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
  this->mapSize = input->mapVoxors.size();

  this->inputFilter = DscConfig::instance()->inputFilter;

  for (auto& mapVoxorId : input->mapVoxors) {
    DCO dco = lwdee::get_dco(mapVoxorId);
    mapDocs.push_back(dco);

    vector<MapRecord> lines;
    mapLines->push_back(lines);
  }

  releaseThread = std::thread(&ToMap::releaseDdo, this);
  releaseThread.detach();
}

uint32_t counter = 0;
void ToMap::accept(RdKafka::Message* message) {
  // logger_trace("< accept kafka offset: %d,%s", message->offset(), static_cast<const char*>(message->payload()));

  counter++;
  try {
    uint64_t now = Stopwatch::currentMilliSeconds();

    string line = string(message->len() + 1, '\0');
    memcpy((void*)line.data(), message->payload(), message->len());

    int index = this->nextMap();
    auto lines = this->mapLines->data() + index;

    if (!inputFilter) {
      LinuxMatrix::stream.kafka_send ++;
      lines->push_back(MapRecord(line, (double)now));
    } else if (inputFilter && counter % 100 == 0) {
      LinuxMatrix::stream.kafka_send ++;
      lines->push_back(MapRecord(line, (double)now));
    }

    if (now != currentTs) {
      currentTs = now;
      this->toMaps();
    }

    // logger_trace("> accept kafka offset: %d,%s", message->offset(), static_cast<const char*>(message->payload()));
  } catch (Exception& ex) {
    logger_error("accept kafka data failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
  } catch (std::exception& ex) {
    logger_error("accept kafka data failed,%s", ex.what());
  }
};

int ToMap::nextMap() {
  currentMap++;
  if (currentMap >= mapLines->size()) {
    currentMap = 0;
  }
  return currentMap;
}

void ToMap::toMaps() {
  for (int i = 0; i < mapSize; i++) {
    this->toMap(i);
  }
}

void ToMap::toMap(int index) {
  auto dco = this->mapDocs.data() + index;
  auto lines = this->mapLines->data() + index;

  auto jsonText = MapInvokeData(input->index, lines).toJson();
  // logger_info("send to map %d lines", lines->size());

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