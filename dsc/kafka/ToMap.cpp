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
  if (mapRecords != nullptr) {
    delete mapRecords;
    mapRecords = nullptr;
  }

  if (ddoIds != nullptr) {
    delete ddoIds;
    ddoIds = nullptr;
  }
}

void ToMap::create_dco(PartitionKafka* input) {
  this->input = input;
  this->mapSize = input->mapVoxors.size();
  this->currentWindowTs = this->getCurrentWindow();

  this->filterCondon = DscConfig::instance()->filterCondon;

  for (auto& mapVoxorId : input->mapVoxors) {
    DCO dco = lwdee::get_dco(mapVoxorId);
    mapDocs.push_back(dco);
  }

  releaseThread = std::thread(&ToMap::releaseDdo, this);
  releaseThread.detach();
}

uint32_t counter = 0;
void ToMap::accept(RdKafka::Message* message) {
  counter++;
  try {
    uint64_t newWindowTs = this->getCurrentWindow();

    mut.lock();

    this->push(message);

    if (newWindowTs != currentWindowTs) {
      currentWindowTs = newWindowTs;
      this->toMaps();
    }

    mut.unlock();

  } catch (Exception& ex) {
    logger_error("accept kafka data failed,%s,%s", ex.getMessage().c_str(), ex.getStackTrace().c_str());
  } catch (std::exception& ex) {
    logger_error("accept kafka data failed,%s", ex.what());
  }
};

void ToMap::push(RdKafka::Message* message) {
  string line = string(message->len() + 1, '\0');
  memcpy((void*)line.data(), message->payload(), message->len());

  uint64_t now = Stopwatch::currentMilliSeconds();

  if (counter % filterCondon == 0) {
    LinuxMatrix::stream.kafka_send++;
    mapRecords->push_back(MapRecord(line, (double)now));
  }
};

void ToMap::toMaps() {
  typedef std::shared_ptr<vector<MapRecord>> MapRecords;

  std::vector<MapRecords> bats;

  int range = this->mapRecords->size() / mapSize;
  int index = -1;

  for (int mapIndex = 0; mapIndex < mapSize; mapIndex++) {
    auto bat = std::make_shared<vector<MapRecord>>();
    bats.push_back(bat);
    for (int i = 0; i < range; i++) {
      index++;
      bat->push_back(this->mapRecords->at(index));
    }
  }

  MapRecords tail = bats.at(mapSize - 1);
  for (; index < this->mapRecords->size(); index++) {
    tail->push_back(this->mapRecords->at(index));
  }

  for (int i = 0; i < mapSize; i++) {
    auto bat = bats[i];
    this->toMap(i, bat.get());

    bat->clear();
  }

  this->mapRecords->clear();
  bats.clear();
}

void ToMap::toMap(int index, vector<MapRecord>* mapLines) {
  auto dco = this->mapDocs.data() + index;
  auto jsonText = MapInvokeData(input->index, mapLines).toJson();

  // logger_debug("send to map %d lines", mapLines->size());

  DDOId ddoId = dco->async("map", jsonText);

  ddoIds->push_back(std::make_pair(ddoId, dco));
}

uint64_t ToMap::getCurrentWindow() {
  uint64_t now = Stopwatch::currentMilliSeconds();
  return now + window - (now % window);
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
