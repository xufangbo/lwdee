#pragma once

#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

typedef char Byte;
class ByteSpan {
 private:
  int pos = 0;

 public:
  int size;
  Byte* buffer = nullptr;

  ByteSpan(int size) : size(size), buffer(new Byte[size]) {}

  ~ByteSpan() {
    if (buffer != nullptr) {
      delete[] buffer;
      buffer = nullptr;
    }
  }

 public:
  void reset() { pos = 0; }
  void put(uint8_t v) { this->puts((Byte*)&v, sizeof(v)); }
  void put(bool v) { this->puts((Byte*)&v, sizeof(v)); }
  void put(double v) { this->puts((Byte*)&v, sizeof(v)); }
  void putInt32(int v) { this->puts((Byte*)&v, sizeof(v)); }
  void put(short v) { this->puts((Byte*)&v, sizeof(v)); }
  void put(int v) { this->puts((Byte*)&v, sizeof(v)); }
  void put(std::string& v) { this->puts((Byte*)v.data(), v.size()); }
  void puts(Byte* ptr, int len) {
    for (int i = 0; i < len; i++) {
      buffer[pos] = ptr[i];
      pos++;
    }
  }

  void read(uint8_t& v) { this->reads((Byte*)&v, sizeof(v)); }
  void read(bool& v) { this->reads((Byte*)&v, sizeof(v)); }
  void read(double& v) { this->reads((Byte*)&v, sizeof(v)); }
  void read(int& v) { this->reads((Byte*)&v, sizeof(v)); }
  void read(short& v) { this->reads((Byte*)&v, sizeof(v)); }
  void readInt32(int& v) { this->reads((Byte*)&v, sizeof(v)); }

  void renew(int len) {
    if (buffer != nullptr) {
      delete[] buffer;
      buffer = nullptr;
    }

    buffer = new Byte[len];
    for (int i = 0; i < len; i++) {
      buffer[i] = 0;
    }
    size = len;
    pos = 0;
  }

  void reads(Byte* ptr, int len) {
    for (int i = 0; i < len; i++) {
      Byte b = buffer[pos];
      ptr[i] = b;
      pos++;
    }
  }

  std::string toHex() {
    std::stringstream out;
    for (int i = 0; i < size; i++) {
      out << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
          << (int)buffer[i];
    }
    out << "  ";

    return out.str();
  }
};
typedef std::shared_ptr<ByteSpan> ByteSpan_ref;