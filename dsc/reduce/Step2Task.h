#pragma once

#include <regex>
#include "core/Partition.h"

class Step2Task {
 private:
  PartitionStep2* input;
  Step2Output output;
  unsigned long size = 0;
  char* content = nullptr;

 public:
  Step2Output run(PartitionStep2* p);
  ~Step2Task() {
    if (content != nullptr) {
      delete[] content;
      content = nullptr;
    }
  }

 private:
  void read();
  void save();

  std::string fileName() {
    char index[] = "data-output-  ";
    sprintf(index, "data-output-%02d", input->index + 1);

    return std::regex_replace(input->outputFile, regex("data-output"), index);
  }
};