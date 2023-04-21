#include "ProtocalFactory.hpp"

#include <memory>

#include "protocal/DefaultProtocal.hpp"
#include "protocal/DefaultStream.hpp"

LeopardProtocalPtr ProtocalFactory::protocal = std::make_shared<DefaultProtocal>();

std::shared_ptr<LeopardProtocal> ProtocalFactory::getProtocal() {
  return protocal;
}

BufferStream* ProtocalFactory::createStream() {
  return new DefaultStream();
}
