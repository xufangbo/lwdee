#include "ProtocalFactory.hpp"

#include <memory>

#include "protocal/DefaultProtocal.hpp"
#include "protocal/DefaultStream.hpp"

LeopardProtocalPtr ProtocalFactory::protocal = std::make_shared<DefaultProtocal>();

std::shared_ptr<LeopardProtocal> ProtocalFactory::getProtocal() {
  return protocal;
}
std::shared_ptr<BufferStream> ProtocalFactory::createStream() {
  return std::make_shared<DefaultStream>();
}