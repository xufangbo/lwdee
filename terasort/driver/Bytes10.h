#pragma once

#include <cstring>
#include <iostream>

class Bytes10 {
 private:
  char values[10];

 public:
  Bytes10(const char* chars) {
    memcpy(values, (void*)chars, 10);
  }

  void operator=(const char* chars) {
    memcpy(values, (void*)chars, 10);
  }

  char* bytes() {
    return this->values;
  }

  bool operator>(const char* chars) {
    return compare(this->bytes(), chars) > 0L;
  }

  bool operator<(const char* chars) {
    return compare(this->bytes(), chars) < 0L;
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
    // std::string str1(11, '\0');
    // memcpy((void*)str1.data(), (void*)this->values, 10);
    // return str1;

    uint64_t l;
    memcpy(&l, (void*)values, 8);

    uint32_t i;
    memcpy(&i, (void*)values + 8, 4);

    std::string str = std::string("") + std::to_string(l) + "-" + std::to_string(i);
    return str;
  }
};