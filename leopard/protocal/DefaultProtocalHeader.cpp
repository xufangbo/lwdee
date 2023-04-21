#include "DefaultProtocalHeader.hpp"

int DefaultProtocalHeader::headerSize() {
  return sizeof(totalLength) + sizeof(sen1) + sizeof(rec1) + sizeof(sen2) + sizeof(rec2) + sizeof(pathLength) + path.size();
}

std::string DefaultProtocalHeader::to_string() {
  std::stringstream s;
  s << "( " << this->sen1 << " -> " << this->rec1 << " -> " << this->sen2 << " -> " << this->rec2 << " )";
  s << " <==> " << std::setiosflags(std::ios::fixed);
  s << "( " << std::setprecision(3) << this->rec1_sen1()
    << " + " << std::setprecision(3) << this->sen2_rec1()
    << " + " << std::setprecision(3) << this->rec2_sen2()
    << " = " << std::setprecision(3) << this->rec2_sen1() << " )";

  return s.str();
}

void DefaultProtocalHeader::setHeader(BufferStream* outputStream, std::string& path, uint64_t sen1, uint32_t rec1, uint32_t sen2, uint32_t rec2) {
  outputStream->put<uint64_t>(0);            // len
  outputStream->put<uint64_t>(sen1);         // sen1
  outputStream->put<uint32_t>(rec1);         // rec1
  outputStream->put<uint32_t>(sen2);         // sen2
  outputStream->put<uint32_t>(rec2);         // rec2
  outputStream->put<uint32_t>(path.size());  // path length
  outputStream->put(path);                   // path
}

void DefaultProtocalHeader::setLength(BufferStream* outputStream) {
  outputStream->set<uint64_t>(0, outputStream->size());
}

void DefaultProtocalHeader::setsen2(BufferStream* outputStream, uint32_t sen2) {
  outputStream->set<uint32_t>(20, sen2);
}

DefaultProtocalHeader DefaultProtocalHeader::parse(BufferStream* inputStream) {
  DefaultProtocalHeader header;

  header.totalLength = inputStream->get<uint64_t>();
  header.sen1 = inputStream->get<uint64_t>();
  header.rec1 = inputStream->get<uint32_t>();
  header.sen2 = inputStream->get<uint32_t>();
  header.rec2 = inputStream->get<uint32_t>();
  header.pathLength = inputStream->get<uint32_t>();
  header.path = inputStream->getString(header.pathLength);

  return header;
}