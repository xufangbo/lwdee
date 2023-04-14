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

    this->start_reduce();

    this->start_map();

    this->start_kafka();
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

void Driver::start_kafka() {
  logger_info("< start kafka");
  Stopwatch sw;

  std::vector<pair<DCO, DDOId>> kafkaInvokers;
  for (int i = 0; i < conf->partitions.size(); i++) {
    printf("\n");
    logger_info("start kafka %d / %d -----------------", i, conf->partitions.size());

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

  for (int i = 0; i < kafkaInvokers.size(); i++) {
    auto& kv = kafkaInvokers[i];
    DCO dco = kv.first;
    DDOId ddoId = kv.second;

    this->get_ddo("kafka ddo", i, kafkaInvokers.size(), dco, ddoId);
  }

  logger_info("> start kafka,eclipse %lf", sw.stop());
}

void Driver::start_map() {
  logger_debug("< map");
  Stopwatch sw;

  std::vector<pair<DCO, DDOId>> mapInvokers;
  for (int i = 0; i < conf->splitNums1; i++) {
    printf("\n");
    logger_debug("start map %d / %d -----------------", i, conf->splitNums1);

    DCO dco = lwdee::create_dco("MapDCO");
    mapVoxorIds.push_back(dco.voxorId());
    logger_debug("map voxorId: %s", dco.voxorId().c_str());

    PartitionMap input(i, reduceVoxorIds);
    DDOId ddoId = dco.async("start", input.toJson());
    logger_debug("%s", input.toJson().c_str());

    mapInvokers.push_back(std::make_pair(dco, ddoId));
  }

  for (int i = 0; i < mapInvokers.size(); i++) {
    auto& kv = mapInvokers[i];
    DCO dco = kv.first;
    DDOId ddoId = kv.second;

    this->get_ddo("map ddo", i, mapInvokers.size(), dco, ddoId);
  }

  logger_debug("> start map,eclipse %lf", sw.stop());
}

void Driver::start_reduce() {
  logger_warn("< start reduce");
  Stopwatch sw;

  std::vector<pair<DCO, DDOId>> reduceInvokers;
  for (int i = 0; i < conf->splitNums2; i++) {
    printf("\n");
    logger_warn("start reduce %d / %d -----------------", i, conf->splitNums2);

    DCO dco = lwdee::create_dco("ReduceDCO");
    reduceVoxorIds.push_back(dco.voxorId());
    logger_warn("reduce voxorId: %s", dco.voxorId().c_str());

    PartitionReduce input(i);
    DDOId ddoId = dco.async("start", input.toJson());
    // logger_warn("%s", input.toJson().c_str());

    reduceInvokers.push_back(std::make_pair(dco, ddoId));
  }

  for (int i = 0; i < reduceInvokers.size(); i++) {
    auto& kv = reduceInvokers[i];
    DCO dco = kv.first;
    DDOId ddoId = kv.second;

    this->get_ddo("reduce ddo", i, reduceInvokers.size(), dco, ddoId);
  }

  logger_warn("> start reduce,eclipse %lf", sw.stop());
}

void Driver::get_ddo(std::string message, int index, size_t size, DCO& dco, DDOId& ddoId) {
  printf("\n");
  try {
    auto ddo = dco.wait(ddoId);
    auto bytes = ddo.read();

    if (::strcmp(bytes->c_str(), "succeed") == 0) {
      logger_warn("%s %d/%d: dco(%s),ddo(%ld),%s", message.c_str(), index, size, dco.voxorId().c_str(), ddoId.itsId(), bytes->c_str());
    } else {
      logger_error("%s %d/%d: dco(%s),ddo(%ld),%s", message.c_str(), index, size, dco.voxorId().c_str(), ddo.ddoId.itsId(), bytes->c_str());
    }

    // ddo.releaseGlobal();
  } catch (Exception& ex) {
    ex.trace(ZONE);
    throw ex;
  }
}