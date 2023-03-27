#pragma once

#include <memory>
#include <vector>

typedef struct {
  char key[10];
  char value[90];

  // TeraRecord() {};
  // TeraRecord(char* ptr) {
  //   memcpy(key, ptr, 10);
  //   memcpy(value, ptr + 10, 90);
  // }

  std::string index() {
    std::string str(11, '\0');
    memcpy((void *)str.data(), value, 10);
    return str;
  }

  uint16_t left2() {
    uint16_t s;
    memcpy(&s, (void *)key, 2);
    return s;
  }

  uint64_t left8() {
    uint64_t l;
    memcpy(&l, (void *)key, 8);
    return l;
  }

  std::string line() {
    std::string line(101, '\0');
    memcpy((void *)line.data(), key, 10);
    memcpy((void *)line.data() + 10, value, 90);
    return line;
  }
} TeraRecord;

typedef std::vector<TeraRecord> TeraRecords;
// typedef std::shared_ptr<std::vector<TeraRecord>> TeraRecords_ref;