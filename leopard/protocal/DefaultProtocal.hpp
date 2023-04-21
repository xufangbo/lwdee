#pragma once

#include <iostream>
#include "net/BufferStream.hpp"
#include "net/Connection.hpp"
#include "net/LeopardProtocal.hpp"

class DefaultProtocal : public LeopardProtocal {
 public:
  BufferStream* client_request(RequestInvoke request, std::string& path) override;
  void client_accept(IRunway* runway, Connection* connection, BufferStream* inputStream) override;
  void server_accept(IRunway* runway, Connection* connection, BufferStream* inputStream) override;
};