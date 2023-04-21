#include "ProtocalFactory.hpp"

#include <memory>

#include "protocal/LeopardProtocal.hpp"
#include "protocal/LeopardStream.hpp"

LeopardProtocalPtr ProtocalFactory::protocal = std::make_shared<LeopardProtocal>();

std::shared_ptr<IProtocal> ProtocalFactory::getProtocal() {
  return protocal;
}

BufferStream* ProtocalFactory::createStream() {
  return new LeopardStream();
}
