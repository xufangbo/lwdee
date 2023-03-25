#pragma once

#include <cstring>
#include <iostream>

class Bytes10 {
 private:
  char values[10];

 public:
  Bytes10(const char* chars) { memcpy(values, (void*)chars, 10); }

  void operator=(const char* chars) { memcpy(values, (void*)chars, 10); }

  char* bytes() { return this->values; }

  bool operator>(const char* chars) {
    return compare(this->bytes(), chars) > 0L;
  }

  bool operator<(const char* chars) {
    return compare(this->bytes(), chars) < 0L;
  }

  uint16_t left2() {
    uint16_t s;
    memcpy(&s, (void*)values, 2);
    return s;
  }

  uint64_t left8() {
    uint64_t l;
    memcpy(&l, (void*)values, 8);
    return l;
  }

  uint32_t right4() {
    uint32_t i;
    memcpy(&i, (void*)values + 8, 4);
    return i;
  }

  /**
   * @brief 比较大小
   * 因为10个字节没说是大小端序，所以不用考虑大小端
   *
   * @param left
   * @param right
   * @return int
   */
  static long compare(char* left, const char* right) {
    uint64_t ll, lr;
    memcpy(&ll, (void*)left, 8);
    memcpy(&lr, (void*)right, 8);

    if (ll != lr) {
      return ll - lr;
    }

    uint32_t il, ir;
    memcpy(&il, (void*)left + 8, 4);
    memcpy(&ir, (void*)right + 8, 4);

    return il - ir;
  }

  std::string to_string() {
    return std::string("") + std::to_string(left8()) + "-" +
           std::to_string(right4());
  }
};