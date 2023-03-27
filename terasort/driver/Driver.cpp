#include "Driver.h"

#include <algorithm>
#include <fstream>
#include <vector>

#include "core/Exception.hpp"
#include "core/Partition.h"
#include "core/log.hpp"
#include "lwdee/DDO.h"
#include "lwdee/lwdee.h"
#include "map/Step1Task.h"
// #include "reduce/Step2Task.h"

void Driver::startJob(std::string fileName, int datum, int splitNums1, int splitNums2) {
  this->fileName = fileName;
  this->datum = datum;
  this->splitNums1 = splitNums1;
  this->splitNums2 = splitNums2;

  try {
    auto conf = this->samples(fileName);
    this->split(conf);
    this->map();
    this->mapToReduce();
    this->reduce();

    logger_info("finished");
  } catch (Exception& ex) {
    logger_error("execute job error,%s", ex.getMessage().c_str());
  }
}

MinAndMax Driver::samples(std::string fileName) {
  // fstream f;
  // f.open(fileName, ios_base::in | ios_base::binary);

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
  logger_debug("total size : %ld,len:%ld", totalSize, len);
  logger_debug("count: %d, min: %s:max: %s", count,
               pair.first.to_string().c_str(), pair.second.to_string().c_str());

  return pair;
}

void Driver::split(MinAndMax conf) {
  // =========================
  // 构造参数
  // =========================
  auto seprator = (conf.second.left8() - conf.first.left8()) / splitNums2;

  uint64_t pre = 0;
  for (int i = 0; i < splitNums2; i++) {
    SampleSplit split;
    split.min = pre;
    split.max = conf.first.left8() + seprator * (i + 1);

    pre = split.max;

    sampleSplits.push_back(split);

    logger_trace("subsplit %d : %ld - %ld", i, split.min, split.max);
  }
  sampleSplits[splitNums2 - 1].max = 0;
}

void Driver::map() {
  // =========================
  // 构造参数
  // =========================
  PartitionStep1 step1Inputs[splitNums1];
  for (int i = 0; i < splitNums1; i++) {
    step1Inputs[i] = PartitionStep1(i, fileName, sampleSplits);
  }

  // =========================
  // 调用map dco
  // =========================

  for (int i = 0; i < splitNums1; i++) {
    DCO dco = lwdee::create_dco(i + 1, "MapDCO");

    PartitionStep1 input(i + 1, fileName, sampleSplits);
    auto json = input.toJson();

    // logger_debug("invoke map.map , index: %d,args: %s",i,json.c_str());

    DDOId ddoId = dco.async("map", json);

    invokers.push_back(std::make_pair(dco, ddoId));
  }
}

void Driver::mapToReduce() {
  for (auto& kv : invokers) {
    DCO dco = kv.first;
    DDOId ddoId = kv.second;

    auto ddo = dco.wait(ddoId);
    auto bytes = ddo.read();

    logger_debug("get map return ddo(%ld),%s", ddo.ddoId.itsId(), bytes->c_str());

    Step1Output setp1Output;
    setp1Output.fromJson(bytes.get());
  }
}

void Driver::reduce() {
  //   // =========================
  //   // step2
  //   // =========================
  //   PartitionStep2 step2Inputs[splitNums2];
  //   for (int i = 0; i < splitNums2; i++) {
  //     step2Inputs[i] = PartitionStep2(i);

  //     for (int j = 0; j < splitNums1; j++) {
  //       step2Inputs[i].ddos.push_back(step1Inputs[j].ddos[i]);
  //     }
  //   }

  //   DDO step2Outpus[splitNums2];
  //   for (int i = 0; i < splitNums2; i++) {
  //     DDO ddo = Step2Task().run(step2Inputs + i);
  //     step2Outpus[i] = ddo;
  //   }

  //   // =========================
  //   // collect
  //   // =========================
  //   Tuples list;
  //   TuplesSerialzer tuplesSerializer;
  //   for (int i = 0; i < splitNums2; i++) {
  //     DDO ddo = step2Outpus[i];
  //     ByteSpan_ref bytes = ddo.read();

  //     ddo.release();
  //     Tuples_ref rc = tuplesSerializer.deserailize(bytes.get());

  //     for_each(rc->begin(), rc->end(), [&list](Tuple &t) { list.push_back(t);
  //     });
  //   }

  //   // =========================
  //   // 打印结果
  //   // =========================
  //   for (Tuple &i : list) {
  //     cout << get<0>(i) << " " << get<1>(i) << endl;
  //   }
}