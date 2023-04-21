#pragma once

#include <iostream>
#include "net/BufferStream.hpp"
#include "net/Connection.hpp"
#include "net/IProtocal.hpp"

class LeopardProtocal : public IProtocal {
 public:
  BufferStream* csend(RequestInvoke request, std::string& path) override;
  void caccept(IRunway* runway, Connection* connection, BufferStream* inputStream) override;
  void saccept(IRunway* runway, Connection* connection, BufferStream* inputStream) override;
};