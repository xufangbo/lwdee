#pragma once

#include "net/BufferStream.hpp"
// #include <memory.h>
// #include <iomanip>
// #include <iostream>
// #include <memory>
// #include <sstream>

#define BUF_UNIT 1024

typedef void byte;

class DefaultStream: public BufferStream {

  bool isEnd() override;
};