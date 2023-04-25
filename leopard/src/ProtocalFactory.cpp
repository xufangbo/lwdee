#include "ProtocalFactory.hpp"

#include <memory>

#include "prot/LeopardProtocal.hpp"
#include "prot/LeopardStream.hpp"

LeopardProtocalPtr ProtocalFactory::protocal = std::make_shared<LeopardProtocal>();

std::shared_ptr<IProtocal> ProtocalFactory::getProtocal() {
  return protocal;
}

BufferStream* ProtocalFactory::createStream() {
  return new LeopardStream();
}
