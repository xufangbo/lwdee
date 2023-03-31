#include "Driver.h"

#include <algorithm>
#include <fstream>
#include <vector>

#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/Stopwatch.h"
#include "core/log.hpp"
#include "driver/TerasortConfig.hpp"
#include "lwdee/DDO.h"
#include "lwdee/lwdee.h"
#include "map/Step1Task.h"

void Driver::startJob() {

  this->inputFile = TerasortConfig::instance()->inputFile;
  this->outputFile = TerasortConfig::instance()->outputFile;
  this->datum = TerasortConfig::instance()->datum;
  this->splitNums1 = TerasortConfig::instance()->splitNums1;
  this->splitNums2 = TerasortConfig::instance()->splitNums2;

  try {
    Stopwatch sw;

    auto conf = this->samples(inputFile);
    this->split(conf);
    this->map();
    this->mapToReduce();
    this->reduce();

    logger_info("finish job,eclipse %lf s", sw.stop());

    logger_info("finished");
  } catch (Exception& ex) {
    logger_error("execute job error,%s", ex.getMessage().c_str());
  }
}

MinAndMax Driver::samples(std::string fileName) {
  logger_info("< samples");
  Stopwatch sw;

  FILE* f = fopen(fileName.c_str(), "rb");
  if (f == NULL) {
    logger_error("can't open file : %s", fileName.c_str());
  }

  fseek(f, 0, SEEK_END);
  long totalSize = ftell(f);
  long len = totalSize / 100;

  char key[11];
  memset(key, '\0', 10);

  fseek(f, 0, SEEK_SET);
  fread(key, 1, 10, f);
  fseek(f, 90L, SEEK_CUR);

  Bytes10 min(key);
  Bytes10 max(key);

  int count = 0;
  for (int i = 0; i < len - 1; i += datum) {
    fread(key, 1, 10, f);
    fseek(f, 100L * (datum - 1) + 90L, SEEK_CUR);

    if (min > key) {
      min = key;
    } else if (max < key) {
      max = key;
    }

    count++;

    // printf("%s,%ld\n", key, ftell(f));
  }

  fclose(f);

  auto pair = std::make_pair(min, max);
  logger_debug("total size : %ld,terarecord count:%ld", totalSize, len);
  logger_debug("count: %d, min: %s:max: %s", count, pair.first.to_string().c_str(), pair.second.to_string().c_str());

  logger_info("> samples,eclipse %lf", sw.stop());

  return pair;
}

void Driver::split(MinAndMax conf) {
  auto seprator = (conf.second.left8() - conf.first.left8()) / splitNums2;

  uint64_t pre = 0;
  for (int i = 0; i < splitNums2; i++) {
    SampleSplit split;
    split.min = pre;
    split.max = conf.first.left8() + seprator * (i + 1);

    pre = split.max;
    if (i + 1 == splitNums2) {
      split.max = 0;
    }

    sampleSplits.push_back(split);

    logger_trace("subsplit %d : %llu - %llu", i, split.min, split.max);
  }
}

void Driver::map() {
  logger_info("< map");
  Stopwatch sw;

  for (int i = 0; i < splitNums1; i++) {
    logger_debug("invoke dco.map, node: %d", (i + 1));

    DCO dco = lwdee::create_dco(i + 2, "MapDCO");

    PartitionStep1 input(i, inputFile, sampleSplits);
    auto json = input.toJson();

    DDOId ddoId = dco.async("map", json);

    step1Invokers.push_back(std::make_pair(dco, ddoId));
  }

  logger_info("> map,eclipse %lf", sw.stop());
}

void Driver::mapToReduce() {
  logger_info("< mapToReduce");
  Stopwatch sw;

  std::vector<Step1Output> step1Outputs;
  for (auto& kv : step1Invokers) {
    DCO dco = kv.first;
    DDOId ddoId = kv.second;

    try {
      auto ddo = dco.wait(ddoId);
      auto bytes = ddo.read();

      // logger_debug("get step 1 ddo(%ld),%s", ddo.ddoId.itsId(), bytes->c_str());

      Step1Output setp1Output;
      setp1Output.fromJson(bytes.get());

      ddo.releaseGlobal();

      step1Outputs.push_back(setp1Output);
    } catch (Exception& ex) {
      ex.trace(ZONE);
    }
  }

  for (int i = 0; i < splitNums2; i++) {
    PartitionStep2 step2Input(i, outputFile);
    for (Step1Output& step1Output : step1Outputs) {
      step2Input.subSplitDDOs.push_back(step1Output.items[i]);
    }

    step2Inputs.push_back(step2Input);
  }

  logger_info("> mapToReduce,eclipse %lf", sw.stop());
}

void Driver::reduce() {
  logger_info("< reduce");
  Stopwatch sw;

  for (int i = 0; i < step2Inputs.size(); i++) {
    PartitionStep2& step2Input = step2Inputs[i];

    DCO dco = lwdee::create_dco(i + 2, "ReduceDCO");
    auto json = step2Input.toJson();

    DDOId ddoId = dco.async("reduce", json);

    step2Invokers.push_back(std::make_pair(dco, ddoId));
  }

  for (auto& kv : step2Invokers) {
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
    }
  }

  logger_info("> reduce,eclipse %lf", sw.stop());
}