#include "Step0Task.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include "core/Stopwatch.h"
#include "core/log.hpp"
#include "lwdee/lwdee.h"
#include "map/MapDCO.h"

MinAndMax Step0Task::run(PartitionStep0* partition) {
  logger_info("< samples task run");
  Stopwatch sw;

  this->partition = partition;

  // textFile
  auto output = this->samples(partition->fileName);

  logger_info("> samples task run,partition: %d,eclipse %lf", partition->index, sw.stop());

  return output;
};

MinAndMax Step0Task::samples(std::string fileName) {

  logger_info("< samples");
  Stopwatch sw;

  FILE* f = fopen(fileName.c_str(), "rb");
  if (f == NULL) {
    logger_error("can't open file : %s", fileName.c_str());
  }

  fseek(f, 0, SEEK_END);
  long len = ftell(f) / 100;
  long splitCount = len / partition->splitNums1;

  long start = splitCount * partition->index;
  long end = (start + splitCount) > len ? len : start + splitCount;

  logger_debug("%d - read range, %ld - %ld,计 %ld 条", partition->index, start, end - 1, end - start);

  char key[11];
  memset(key, '\0', 10);

  fseek(f, 0, SEEK_SET);
  fread(key, 1, 10, f);
  fseek(f, 90L, SEEK_CUR);

  Bytes10 min(key);
  Bytes10 max(key);

  int datum = partition->datum;

  fseek(f, start * 100, SEEK_SET);
  for (int i = start; i < end; i += datum) {
    fread(key, 1, 10, f);
    fseek(f, 100L * (datum - 1) + 90L, SEEK_CUR);

    if (min > key) {
      min = key;
    } else if (max < key) {
      max = key;
    }
  }

  fclose(f);

  auto pair = std::make_pair(min, max);

  logger_info("> samples,partition: %d,eclipse %lf", partition->index, sw.stop());

  return pair;
}