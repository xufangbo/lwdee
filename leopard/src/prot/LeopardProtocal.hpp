#pragma once

#include <iostream>
#include "src/BufferStream.hpp"
#include "src/Connection.hpp"
#include "src/IProtocal.hpp"

class LeopardProtocal : public IProtocal {
 public:
  BufferStream* newStream(std::string& path) override;
  uint64_t csend(ClientConnection* connection, BufferStream* outputStream) override;
  void caccept(Connection* connection, BufferStream* inputStream) override;
  void saccept(Connection* connection, BufferStream* inputStream) override;
};