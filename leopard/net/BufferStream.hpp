#pragma once

#include <memory.h>

#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

#define BUF_UNIT 4096

typedef void Byte;

class BufferStream {
 protected:
  size_t pos = 0;
  size_t _size = 0;
  size_t capacity = BUF_UNIT;

 public:
  Byte* buffer = NULL;

 public:
  BufferStream();
  BufferStream(size_t len);
  ~BufferStream();

 protected:
  virtual BufferStream* newInstance(size_t len) = 0;
  void realloc(int newSize);

 public:
  template <typename T>
  void put(T v) {
    this->puts((Byte*)&v, sizeof(v));
  }
  void put(std::string& v) { this->puts((Byte*)v.data(), v.size()); }
  void put(const char* v) { this->puts((Byte*)v, strlen(v)); }
  void puts(Byte* buf, int len);

  template <typename T>
  void set(size_t position, T v) {
    memcpy(buffer + position, &v, sizeof(v));
  }
  void set(size_t position, Byte* buf, size_t len) {
    memcpy(buffer + position, buf, len);
  }

 public:
  template <typename T>
  T get() {
    T v;
    this->get((Byte*)&v, sizeof(v));
    return v;
  }
  template <typename T>
  void get(T& v) {
    this->get((Byte*)&v, sizeof(v));
  }
  template <typename T>
  void get(int position, T& v) {
    memcpy((Byte*)&v, buffer + position, sizeof(v));
  }
  void get(std::string& str, int len);
  std::string getString(int len);
  void get(Byte* ptr, int len);

 public:
  void moveToHead();

  virtual bool isEnd() = 0;
  virtual size_t currentSize() = 0;
  BufferStream* pick();

 public:
  size_t size() { return _size; }
  size_t position() { return pos; }
  std::string toHex();
};

typedef std::shared_ptr<BufferStream> BufferStreamPtr;