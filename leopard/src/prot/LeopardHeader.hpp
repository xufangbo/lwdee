#pragma once

#include <atomic>
#include "src/BufferStream.hpp"

class LeopardHeader {
 private:
  static std::atomic<uint64_t> seq;

 public:
  uint64_t totalLength;
  uint64_t messageId;
  uint64_t sen1;
  uint32_t rec1;
  uint32_t sen2;
  uint32_t rec2;
  uint32_t pathLength;
  std::string path;

 public:
  static int INDEX_TOTAL_LENGTH;
  static int INDEX_MESSAGE_ID;
  static int INDEX_SEN1;
  static int INDEX_REC1;
  static int INDEX_SEN2;
  static int INDEX_REC2;
  static int INDEX_PATH_LENGTH;
  static int INDEX_PATH;

 public:
  static uint64_t create(BufferStream* outputStream, std::string& path);
  /**
   * @brief copy request header to response header
   */
  static void copyRequest(BufferStream* outputStream, LeopardHeader& src);
  static void setLength(BufferStream* outputStream);
  static uint64_t getLength(BufferStream* inputStream);
  static uint64_t getMessageId(BufferStream* inputStream);
  void setsen2(BufferStream* outputStream);
  static LeopardHeader parse(BufferStream* inputStream);
  float rec1_sen1() { return rec1 * 1.0 / 1000; }
  float rec2_sen2() { return (rec2 - sen2) * 1.0 / 1000; }
  float sen2_rec1() { return (sen2 - rec1) * 1.0 / 1000; }
  float rec2_sen1() { return rec2 * 1.0 / 1000; }
  int headerSize();
  std::string to_string();
  static std::string to_hex(BufferStream* stream);
};