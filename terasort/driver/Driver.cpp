#include "Driver.h"

#include <algorithm>
#include <fstream>
#include <vector>

#include "core/Partition.h"
#include "core/log.hpp"
// #include "worker/Step1Task.h"
// #include "worker/Step2Task.h"

void Driver::startJob(std::string fileName, int splitNums1, int splitNums2) {
  this->samples(fileName, 3);

  //   // =========================
  //   // 构造参数
  //   // =========================

  //   PartitionStep1 step1Inputs[splitNums1];
  //   for (int i = 0; i < splitNums1; i++) {
  //     step1Inputs[i] = PartitionStep1(i, fileName, splitNums2);
  //   }

  //   // =========================
  //   // step1
  //   // =========================
  //   PartitionStep1 step1Outputs[splitNums1];
  //   for (int i = 0; i < splitNums1; i++) {
  //     PartitionStep1 step1Output = Step1Task().run(step1Inputs + i);
  //     step1Outputs[i] = step1Output;
  //   }

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

  //     for_each(rc->begin(), rc->end(), [&list](Tuple &t) { list.push_back(t); });
  //   }

  //   // =========================
  //   // 打印结果
  //   // =========================
  //   for (Tuple &i : list) {
  //     cout << get<0>(i) << " " << get<1>(i) << endl;
  //   }
}

MinAndMax Driver::samples(std::string fileName, int datum) {
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

    printf("%s,%ld\n", key, ftell(f));
  }

  fclose(f);

  auto pair = std::make_pair(min, max);
  logger_warn("total size : %ld,len:%ld", totalSize, len);
  logger_debug("count: %d, min: %s:max: %s", count, pair.first.to_string().c_str(), pair.second.to_string().c_str());
  return pair;
}

// MinAndMax Driver::samples(std::string fileName, int datum) {
//   fstream f;
//   f.open(fileName, ios_base::in | ios_base::binary);

//   if (!f.is_open()) {
//     logger_error("can't open file : %s", fileName.c_str());
//   }

//   f.seekg(0, ios_base::end);
//   uint64_t totalSize = f.tellg();
//   uint64_t len = totalSize / 100;

//   logger_warn("total size : %ld,len:%ld", totalSize, len);

//   char key[11];
//   memset(key, '\0', 10);

//   f.seekg(0, ios_base::beg);
//   f.read(key, 10);
//   f.seekg(90L, ios_base::cur);

//   Bytes10 min(key);
//   Bytes10 max(key);

//   int count = 0;
//   for (int i = 0; !f.eof(); i += datum) {
//     f.read(key, 10);
//     f.seekg(100L * datum, ios_base::cur);
//     f.seekg(90L, ios_base::cur);

//     if (min > key) {
//       min = key;
//     } else if (max < key) {
//       max = key;
//     }

//     count++;

//     printf("%s,%ld\n", key, (uint64_t)f.tellg());
//   }

//   f.close();

//   auto pair = std::make_pair(min, max);
//   logger_debug("count: %d, min: %s:max: %s",count, pair.first.to_string().c_str(), pair.second.to_string().c_str());
//   return pair;
// }
