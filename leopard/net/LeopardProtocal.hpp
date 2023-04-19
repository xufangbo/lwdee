#pragma once

#include "BufferStream.hpp"

class ProtocalHeader {
 public:
  uint64_t totalLength;
  uint64_t sen1;
  uint32_t rec1;
  uint32_t sen2;
  uint32_t rec2;
  uint32_t pathLength;
  std::string path;

  int headerSize() {
    return sizeof(totalLength) + sizeof(sen1) + sizeof(rec1) + sizeof(sen2) + sizeof(rec2) + sizeof(pathLength) + path.size();
  }

 public:
  float rec1_sen1() { return rec1 * 1.0 / 1000; }
  float rec2_sen2() { return (rec2 - sen2) * 1.0 / 1000; }
  float sen2_rec1() { return (sen2 - rec1) * 1.0 / 1000; }
  float rec2_sen1() { return rec2 * 1.0 / 1000; }

  std::string to_string() {
    std::stringstream s;
    s << "( " << this->sen1 << " -> " << this->rec1 << " -> " << this->sen2 << " -> " << this->rec2 << " )";
    s << " <==> " << std::setiosflags(std::ios::fixed);
    s << "( " << std::setprecision(3) << this->rec1_sen1()
      << " + " << std::setprecision(3) << this->sen2_rec1()
      << " + " << std::setprecision(3) << this->rec2_sen2()
      << " = " << std::setprecision(3) << this->rec2_sen1() << " )";

    return s.str();
  }
};

typedef std::shared_ptr<ProtocalHeader> ProtocalHeaderPtr;

class LeopardProtocal {
 public:
  virtual void setHeader(BufferStream* outputStream, std::string& path, uint64_t sen1, uint32_t rec1, uint32_t sen2, uint32_t rec2) = 0;
  virtual void setLength(BufferStream* outputStream) = 0;
  virtual void setsen2(BufferStream* outputStream, uint32_t sen2) = 0;
  virtual ProtocalHeaderPtr getHeader(BufferStream* inputStream) = 0;
};

typedef std::shared_ptr<LeopardProtocal> LeopardProtocalPtr;