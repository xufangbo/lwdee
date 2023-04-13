#include "Driver.h"

#include <algorithm>
#include <fstream>
#include <vector>

#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/Stopwatch.h"
#include "core/log.hpp"
#include "lwdee/DDO.h"
#include "lwdee/lwdee.h"
#include "matrix/LinuxMatrix.h"

void Driver::startJob() {
  try {
    logger_info("> job");
    // LinuxMatrix::print();

    this->conf = DscConfig::instance();

    if (conf->partitions.size() > conf->splitNums1) {
      throw Exception("kafka partiton cout can't be more than map partitons cout", ZONE);
    }

    Stopwatch sw;

    this->startReduce();

    this->startMap();

    this->startKafka();
    // this->mapToReduce();

    // LinuxMatrix::print();
    logger_info("< job,eclipse %lf s", sw.stop());

    logger_info("finished");
  } catch (Exception& ex) {
    logger_error("execute driver error,%s", ex.getMessage().c_str());
  } catch (std::exception& ex) {
    logger_error("execute driver error,%s", ex.what());
  }
}

void Driver::startKafka() {
  logger_info("< start kafka");
  Stopwatch sw;

  std::vector<pair<DCO, DDOId>> kafkaInvokers;
  for (int i = 0; i < conf->partitions.size(); i++) {
    logger_info("start kafka %d -----------------", i);

    std::vector<string> kafka_mapVoxors;
    for (int m = i; m < conf->splitNums1; m = m + conf->partitions.size()) {
      kafka_mapVoxors.push_back(mapVoxorIds[m]);
    }
    PartitionKafka input(i, conf->group, conf->topic, conf->window, kafka_mapVoxors);

    DCO dco = lwdee::create_dco("KafkaDCO");
    logger_info("kafka voxorId: %s", dco.voxorId().c_str());

    DDOId ddoId = dco.async("start", input.toJson());
    logger_info("%s", input.toJson().c_str());

    kafkaInvokers.push_back(std::make_pair(dco, ddoId));
  }

  for (auto& kv : kafkaInvokers) {
    DCO dco = kv.first;
    DDOId ddoId = kv.second;

    try {
      auto ddo = dco.wait(ddoId);
      auto bytes = ddo.read();

      logger_info("get kafka start ddo(%ld),%s", ddo.ddoId.itsId(), bytes->c_str());

      ddo.releaseGlobal();
    } catch (Exception& ex) {
      ex.trace(ZONE);
      throw ex;
    }
  }

  logger_info("> start kafka,eclipse %lf", sw.stop());
}

void Driver::startMap() {
  logger_debug("< map");
  Stopwatch sw;

  std::vector<pair<DCO, DDOId>> mapInvokers;
  for (int i = 0; i < conf->splitNums1; i++) {
    logger_debug("start map %d -----------------", i);

    DCO dco = lwdee::create_dco("MapDCO");
    mapVoxorIds.push_back(dco.voxorId());
    logger_debug("map voxorId: %s", dco.voxorId().c_str());

    PartitionMap input(i, reduceVoxorIds);
    DDOId ddoId = dco.async("start", input.toJson());
    logger_debug("%s", input.toJson().c_str());

    mapInvokers.push_back(std::make_pair(dco, ddoId));
  }

  for (auto& kv : mapInvokers) {
    DCO dco = kv.first;
    DDOId ddoId = kv.second;

    try {
      auto ddo = dco.wait(ddoId);
      auto bytes = ddo.read();

      logger_debug("get map start ddo(%ld),%s", ddo.ddoId.itsId(), bytes->c_str());

      ddo.releaseGlobal();

    } catch (Exception& ex) {
      ex.trace(ZONE);
      throw ex;
    }
  }

  logger_debug("> start map,eclipse %lf", sw.stop());
}

void Driver::startReduce() {
  logger_trace("< start reduce");
  Stopwatch sw;

  std::vector<pair<DCO, DDOId>> reduceInvokers;
  for (int i = 0; i < conf->splitNums2; i++) {
    logger_trace("start reduce %d -----------------", i);

    DCO dco = lwdee::create_dco("ReduceDCO");
    reduceVoxorIds.push_back(dco.voxorId());
    logger_trace("reduce voxorId: %s", dco.voxorId().c_str());

    PartitionReduce input(i);
    DDOId ddoId = dco.async("start", input.toJson());
    logger_trace("%s", input.toJson().c_str());

    reduceInvokers.push_back(std::make_pair(dco, ddoId));
  }

  for (auto& kv : reduceInvokers) {
    DCO dco = kv.first;
    DDOId ddoId = kv.second;

    try {
      auto ddo = dco.wait(ddoId);
      auto bytes = ddo.read();

      logger_trace("get reduce start : ddo(node%d),%s", ddo.ddoId.itsWorkNodeId(), bytes->c_str());

      ddo.releaseGlobal();
    } catch (Exception& ex) {
      ex.trace(ZONE);
      throw ex;
    }
  }

  logger_trace("> start reduce,eclipse %lf", sw.stop());
}