#pragma once

#include <memory.h>

#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

#define BUF_UNIT 1024

typedef void byte;

class BufferStream {
 protected:
  size_t pos = 0;
  size_t _size = 0;
  size_t capacity = BUF_UNIT;

 public:
  byte* buffer = NULL;

 public:
  BufferStream();
  ~BufferStream();

 public:
  template <typename T>
  void put(T v) {
    this->puts((byte*)&v, sizeof(v));
  }
  void put(std::string& v) { this->puts((byte*)v.data(), v.size()); }
  void put(const char* v) { this->puts((byte*)v, strlen(v)); }
  void puts(byte* buf, int len);

  template <typename T>
  void set(size_t position, T v) {
    memcpy(buffer + position, &v, sizeof(v));
  }
  void set(size_t position, byte* buf, size_t len) {
    memcpy(buffer + position, buf, len);
  }

 public:
  template <typename T>
  T get() {
    T v;
    this->get((byte*)&v, sizeof(v));
    return v;
  }
  template <typename T>
  void get(T& v) {
    this->get((byte*)&v, sizeof(v));
  }
  template <typename T>
  void get(int position, T& v) {
    memcpy((byte*)&v, buffer + position, sizeof(v));
  }
  void get(std::string& str, int len);
  std::string getString(int len);
  void get(byte* ptr, int len);

 public:
  void reset() { pos = 0; }
  void clean();

  virtual bool isEnd() = 0;

 public:
  size_t size() { return _size; }
  std::string toHex();
};

typedef std::shared_ptr<BufferStream> BufferStreamPtr;