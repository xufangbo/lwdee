#pragma once

#include <iostream>
#include "net/BufferStream.hpp"
#include "net/Connection.hpp"
#include "net/IProtocal.hpp"

class LeopardProtocal : public IProtocal {
 public:
  BufferStream* newStream( std::string& path) override;
  void send(IRunway* runway, Connection* connection, BufferStream* outputStream) override;
  void caccept(IRunway* runway, Connection* connection, BufferStream* inputStream) override;  
  void saccept(IRunway* runway, Connection* connection, BufferStream* inputStream) override;
};