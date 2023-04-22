#pragma once

#include "net/BufferStream.hpp"

class LeopardHeader {
 public:
  uint64_t totalLength;
  uint64_t sen1;
  uint32_t rec1;
  uint32_t sen2;
  uint32_t rec2;
  uint32_t pathLength;
  std::string path;

 public:
  static void setHeader(BufferStream* outputStream, std::string& path, uint64_t sen1, uint32_t rec1, uint32_t sen2, uint32_t rec2);
  static void setLength(BufferStream* outputStream);
  static uint64_t getLength(BufferStream* inputStream);
  static void setsen2(BufferStream* outputStream, uint32_t sen2);
  static LeopardHeader parse(BufferStream* inputStream);
  float rec1_sen1() { return rec1 * 1.0 / 1000; }
  float rec2_sen2() { return (rec2 - sen2) * 1.0 / 1000; }
  float sen2_rec1() { return (sen2 - rec1) * 1.0 / 1000; }
  float rec2_sen1() { return rec2 * 1.0 / 1000; }
  int headerSize();
  std::string to_string();
};