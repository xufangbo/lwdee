#pragma once

#include "BufferStream.hpp"
#include "IProtocal.hpp"

class ProtocalFactory {
 private:
  static LeopardProtocalPtr protocal;

 public:
  static LeopardProtocalPtr getProtocal();
  static BufferStream* createStream();
};