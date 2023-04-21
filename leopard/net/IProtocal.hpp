#pragma once

#include "BufferStream.hpp"
#include "Connection.hpp"
#include "IRunway.hpp"

typedef std::function<void(BufferStream*)> RequestInvoke;
typedef std::function<void(BufferStream*)> RequestCallback;

class IProtocal {
 public:
  virtual BufferStream* csend(RequestInvoke request, std::string& path) = 0;
  virtual void caccept(IRunway* runway, Connection* connection, BufferStream* inputStream) = 0;
  virtual void saccept(IRunway* runway, Connection* connection, BufferStream* inputStream) = 0;
};

typedef std::shared_ptr<IProtocal> LeopardProtocalPtr;