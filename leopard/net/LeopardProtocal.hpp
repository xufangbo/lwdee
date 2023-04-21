#pragma once

#include "BufferStream.hpp"
#include "Connection.hpp"
#include "IRunway.hpp"

typedef std::function<void(BufferStream*)> RequestInvoke;
typedef std::function<void(BufferStream*)> RequestCallback;

class LeopardProtocal {
 public:
  virtual BufferStream* client_request(RequestInvoke request, std::string& path) = 0;
  virtual void client_accept(IRunway* runway, Connection* connection, BufferStream* inputStream) = 0;
  virtual void server_accept(IRunway* runway, Connection* connection, BufferStream* inputStream) = 0;
};

typedef std::shared_ptr<LeopardProtocal> LeopardProtocalPtr;