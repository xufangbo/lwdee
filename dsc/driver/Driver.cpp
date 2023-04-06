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
#include "map/Step1Task.h"
#include "matrix/LinuxMatrix.h"

void Driver::startJob() {
  logger_info("> job");
  LinuxMatrix::print();

  this->conf = DscConfig::instance();

  try {
    Stopwatch sw;

    this->kafka();
    this->kafkaToMap();
    // this->map();
    // this->mapToReduce();

    LinuxMatrix::print();
    logger_info("< job,eclipse %lf s", sw.stop());

    logger_info("finished");
  } catch (Exception& ex) {
    logger_error("execute job error,%s", ex.getMessage().c_str());
  }
}

void Driver::kafka() {
  logger_info("< kafka");
  Stopwatch sw;

  for (int& i : conf->partitions) {
    logger_debug("invoke dco.kafka, split: %d", (i + 1));

    DCO dco = lwdee::create_dco("KafkaDCO");
    // DCO dco = lwdee::create_dco(i + 2, "MapDCO");

    PartitionKafka input(i, conf->group, conf->topic);
    auto json = input.toJson();

    DDOId ddoId = dco.async("start", json);

    kafkaInvokers.push_back(std::make_pair(dco, ddoId));
  }

  logger_info("> kafka,eclipse %lf", sw.stop());
}

void Driver::kafkaToMap() {
  logger_info("< kafkaToMap");
  Stopwatch sw;

  std::vector<std::string> step1Outputs;
  for (auto& kv : kafkaInvokers) {
    DCO dco = kv.first;
    DDOId ddoId = kv.second;

    try {
      auto ddo = dco.wait(ddoId);
      auto bytes = ddo.read();

      logger_debug("get kafka ddo(%ld),%s", ddo.ddoId.itsId(), bytes->c_str());

      ddo.releaseGlobal();

      step1Outputs.push_back(*bytes);
    } catch (Exception& ex) {
      ex.trace(ZONE);
      throw ex;
    }
  }

  for (int i = 0; i < conf->splitNums2; i++) {
    PartitionReduce step2Input(i, conf->outTopic);

    for (std::string& output1 : step1Outputs) {
      // step2Input.subSplitDDOs.push_back(step1Output.items[i]);
    }

    step2Inputs.push_back(step2Input);
  }

  logger_info("> kafkaToMap,eclipse %lf", sw.stop());
}

void Driver::map() {
  logger_info("< map");
  Stopwatch sw;

  for (int i = 0; i < conf->splitNums1; i++) {
    logger_debug("invoke dco.map, split: %d", (i + 1));

    DCO dco = lwdee::create_dco("MapDCO");
    // DCO dco = lwdee::create_dco(i + 2, "MapDCO");

    PartitionMap input(i, conf->inputFile);
    auto json = input.toJson();

    DDOId ddoId = dco.async("start", json);

    mapInvokers.push_back(std::make_pair(dco, ddoId));
  }

  logger_info("> map,eclipse %lf", sw.stop());
}

void Driver::mapToReduce() {
  logger_info("< mapToReduce");
  Stopwatch sw;

  std::vector<std::string> step1Outputs;
  for (auto& kv : mapInvokers) {
    DCO dco = kv.first;
    DDOId ddoId = kv.second;

    try {
      auto ddo = dco.wait(ddoId);
      auto bytes = ddo.read();

      logger_debug("get step 1 ddo(%ld),%s", ddo.ddoId.itsId(), bytes->c_str());

      ddo.releaseGlobal();

      step1Outputs.push_back(*bytes);
    } catch (Exception& ex) {
      ex.trace(ZONE);
      throw ex;
    }
  }

  for (int i = 0; i < conf->splitNums2; i++) {
    PartitionReduce step2Input(i, conf->outTopic);

    for (std::string& output1 : step1Outputs) {
      // step2Input.subSplitDDOs.push_back(step1Output.items[i]);
    }

    step2Inputs.push_back(step2Input);
  }

  logger_info("> mapToReduce,eclipse %lf", sw.stop());
}

void Driver::reduce() {
  logger_info("< reduce");
  Stopwatch sw;

  for (int i = 0; i < step2Inputs.size(); i++) {
    logger_debug("invoke dco.reduce, split: %d", (i + 1));

    PartitionReduce& step2Input = step2Inputs[i];

    DCO dco = lwdee::create_dco("ReduceDCO");
    // DCO dco = lwdee::create_dco(i + 2, "ReduceDCO");
    auto json = step2Input.toJson();

    DDOId ddoId = dco.async("reduce", json);

    reduceInvokers.push_back(std::make_pair(dco, ddoId));
  }

  for (auto& kv : reduceInvokers) {
    DCO dco = kv.first;
    DDOId ddoId = kv.second;

    try {
      auto ddo = dco.wait(ddoId);
      auto bytes = ddo.read();

      logger_debug("get step 2 ddo(node%d),%s", ddo.ddoId.itsWorkNodeId(), bytes->c_str());

      // Step1Output setp1Output;
      // setp1Output.fromJson(bytes.get());

      ddo.releaseGlobal();
    } catch (Exception& ex) {
      ex.trace(ZONE);
      throw ex;
    }
  }

  logger_info("> reduce,eclipse %lf", sw.stop());
}