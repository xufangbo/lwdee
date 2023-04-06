#pragma once

#include <cstring>
#include <iostream>
#include <algorithm>

class Bytes10 {
 private:
  char values[10];

 public:
  Bytes10(const char* chars) { memcpy(values, (void*)chars, 10); }

  void operator=(const char* chars) { memcpy(values, (void*)chars, 10); }

  char* bytes() { return this->values; }

  bool operator>(const char* chars) {
    // return compare(this->bytes(), chars) > 0L;
    return memcmp(values,chars, 8) > 0;
  }

  bool operator<(const char* chars) {
    // return compare(this->bytes(), chars) < 0L;
    return memcmp(values, chars, 8) < 0;
  }

 uint16_t left2() {
    uint16_t s;
    memcpy(&s, (void*)values, 2);

    char* ptr = (char*)&s;
    std::reverse(ptr, ptr + 2);

    return s;
  }

  uint64_t left8() {
    uint64_t l;
    memcpy(&l, (void*)values, 8);

    char* ptr = (char*)&l;
    std::reverse(ptr, ptr + 8);

    return l;
  }

  uint16_t right2() {
    uint16_t i;
    memcpy(&i, (void*)(values + 8), 2);

    char* ptr = (char*)&i;
    std::reverse(ptr, ptr + 2);

    return i;
  }

  // /**
  //  * @brief 比较大小
  //  * 因为10个字节没说是大小端序，所以不用考虑大小端
  //  *
  //  * @param left
  //  * @param right
  //  * @return int
  //  */
  // static long compare(char* left, const char* right) {
  //   uint64_t ll, lr;
  //   memcpy(&ll, (void*)left, 8);
  //   memcpy(&lr, (void*)right, 8);

  //   if (ll != lr) {
  //     return ll - lr;
  //   }

  //   uint32_t il, ir;
  //   memcpy(&il, (void*)left + 8, 2);
  //   memcpy(&ir, (void*)right + 8, 2);

  //   return il - ir;
  // }

  std::string to_string() {
    return std::string("") + std::to_string(left8()) + "-" +
           std::to_string(right2());
  }
};