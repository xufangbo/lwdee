#include "ToMap.h"

#include <time.h>
#include "core/DscConfig.hpp"
#include "core/Exception.hpp"
#include "core/Stopwatch.hpp"
#include "core/cjson.hpp"
#include "core/log.hpp"
#include "map/MapDCO.h"
#include "matrix/LinuxMatrix.h"
#include "client/SocketScheduler.hpp"
#include "client/TcpRequest.hpp"
#include "core/NodeConfig.hpp"


ToMap::ToMap() {
}

ToMap::~ToMap() {
  if (mapRecords != nullptr) {
    delete mapRecords;
    mapRecords = nullptr;
  }
}

void ToMap::create_dco(PartitionKafka* input) {
  this->input = input;
  this->mapSize = input->mapVoxors.size();
  this->currentWindowTs = this->getCurrentWindow();

  this->filterCondon = DscConfig::instance()->filterCondon;

  for (auto& mapVoxorId : input->mapVoxors) {
    auto dco = NodeConfig::voxorId(mapVoxorId);
    mapDocs.push_back(dco);
  }
}

void ToMap::accept(RdKafka::Message* message) {
  try {
    mut.lock();

    this->push(message);

    uint64_t newWindowTs = this->getCurrentWindow();
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

uint32_t counter = 0;
void ToMap::push(RdKafka::Message* message) {
  counter++;
  if (counter >= INT32_MAX) {
    counter = 0;
  }

  uint64_t now = Stopwatch::currentMilliSeconds();

  if (counter % filterCondon == 0) {
    LinuxMatrix::stream.kafka_send++;

    string line = string(message->len() + 1, '\0');
    memcpy((void*)line.data(), message->payload(), message->len());
    mapRecords->push_back(MapRecord(line, (double)now));
  }
};

void ToMap::toMaps() {
  typedef std::shared_ptr<vector<MapRecord>> MapRecords;
  std::vector<MapRecords> bats;

  logger_debug("kafka partition %d QPS: %d", this->input->index, mapRecords->size());
  LinuxMatrix::stream.kafka_qps =  mapRecords->size();

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
  // auto dco = this->mapDocs.data() + index;
  VoxorId voxorId = this->mapDocs[index];
  TNode* node = NodeConfig::byNodeId(voxorId.nodeId);

  // logger_debug("send to map %d lines", mapLines->size());

  auto client = SocketScheduler::newClient(node->ip.c_str(), node->port);
  RequestCallback callback = [](BufferStream* inputStream) {
    auto len = inputStream->get<uint32_t>();
    auto content = inputStream->getString(len);

    // logger_debug("recive(%d) :  %s", len, content.c_str());
  };

  auto json = MapInvokeData(input->index,voxorId.voxorKey, mapLines).toJson();
  client->invoke(ServicePaths::map_invoke, (void*)json.c_str(), json.size(), callback);
  // client->wait();
}

uint64_t ToMap::getCurrentWindow() {
  uint64_t now = Stopwatch::currentMilliSeconds();
  return now + window - (now % window);
}