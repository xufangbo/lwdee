#include "LeopardHeader.hpp"

#include "core/Stopwatch.hpp"

std::atomic<uint64_t> LeopardHeader::seq = 0;
int LeopardHeader::INDEX_TOTAL_LENGTH = 0;
int LeopardHeader::INDEX_MESSAGE_ID = 8;
int LeopardHeader::INDEX_SEN1 = 16;
int LeopardHeader::INDEX_REC1 = 24;
int LeopardHeader::INDEX_SEN2 = 28;
int LeopardHeader::INDEX_REC2 = 32;
int LeopardHeader::INDEX_PATH_LENGTH = 36;
int LeopardHeader::INDEX_PATH = 40;

uint64_t LeopardHeader::create(BufferStream* outputStream, std::string& path) {
  seq++;
  uint64_t i = seq.load();

  outputStream->put<uint64_t>(0);                                 // len
  outputStream->put<uint64_t>(i);                                 // messageId
  outputStream->put<uint64_t>(Stopwatch::currentMilliSeconds());  // sen1
  outputStream->put<uint32_t>(0);                                 // rec1
  outputStream->put<uint32_t>(0);                                 // sen2
  outputStream->put<uint32_t>(0);                                 // rec2
  outputStream->put<uint32_t>(path.size());                       // path length
  outputStream->put(path);                                        // path

  // uint32_t pathLen = outputStream->getPosition<uint32_t>(INDEX_PATH);
  // if (pathLen != path.size()) {
  //   throw std::logic_error("Division by zero condition!");
  // }

  // printf("%s\n",to_hex(outputStream).c_str());

  return i;
}

void LeopardHeader::setLength(BufferStream* outputStream) {
  outputStream->set<uint64_t>(INDEX_TOTAL_LENGTH, outputStream->size());
}

uint64_t LeopardHeader::getLength(BufferStream* inputStream) {
  uint64_t len;
  inputStream->get<uint64_t>(INDEX_TOTAL_LENGTH, len);
  return len;
}

uint64_t LeopardHeader::getMessageId(BufferStream* inputStream) {
  uint64_t msgid;
  inputStream->get<uint64_t>(INDEX_MESSAGE_ID, msgid);
  return msgid;
}

void LeopardHeader::setsen2(BufferStream* outputStream) {
  // uint32_t sen2 = Stopwatch::currentMilliSeconds() - this->sen1;
  // outputStream->set<uint32_t>(INDEX_SEN2, sen2);  // 38
}

LeopardHeader LeopardHeader::parse(BufferStream* inputStream) {
  LeopardHeader header;

  header.totalLength = inputStream->get<uint64_t>();
  header.messageId = inputStream->get<uint64_t>();
  header.sen1 = inputStream->get<uint64_t>();
  header.rec1 = inputStream->get<uint32_t>();
  header.sen2 = inputStream->get<uint32_t>();
  header.rec2 = inputStream->get<uint32_t>();
  header.pathLength = inputStream->get<uint32_t>();
  header.path = inputStream->getString(header.pathLength);

  return header;
}

void LeopardHeader::copyRequest(BufferStream* outputStream,
                                LeopardHeader& src) {
  auto rec1 = Stopwatch::currentMilliSeconds() - src.sen1;
  outputStream->put<uint64_t>(0);               // len
  outputStream->put<uint64_t>(src.messageId);   // messageId
  outputStream->put<uint64_t>(src.sen1);        // sen1
  outputStream->put<uint32_t>(rec1);            // rec1
  outputStream->put<uint32_t>(0);               // sen2
  outputStream->put<uint32_t>(0);               // rec2
  outputStream->put<uint32_t>(src.pathLength);  // path length
  outputStream->put(src.path);                  // path
}

int LeopardHeader::headerSize() {
  return sizeof(totalLength) + sizeof(messageId) + sizeof(sen1) + sizeof(rec1) +
         sizeof(sen2) + sizeof(rec2) + sizeof(pathLength) + path.size();
}

std::string LeopardHeader::to_string() {
  std::stringstream s;
  s << "( " << this->sen1 << " -> " << this->rec1 << " -> " << this->sen2
    << " -> " << this->rec2 << " )";
  s << " <==> " << std::setiosflags(std::ios::fixed);
  s << "( " << std::setprecision(3) << this->rec1_sen1() << " + "
    << std::setprecision(3) << this->sen2_rec1() << " + "
    << std::setprecision(3) << this->rec2_sen2() << " = "
    << std::setprecision(3) << this->rec2_sen1() << " )";

  return s.str();
}

std::string LeopardHeader::to_hex(BufferStream* stream) {
  std::stringstream s;

  uint64_t totalLength = stream->getPosition<uint64_t>(INDEX_TOTAL_LENGTH);
  uint64_t messageId = stream->getPosition<uint64_t>(INDEX_MESSAGE_ID);
  uint64_t sen1 = stream->getPosition<uint64_t>(INDEX_SEN1);
  uint32_t rec1 = stream->getPosition<uint32_t>(INDEX_REC1);
  uint32_t sen2 = stream->getPosition<uint32_t>(INDEX_SEN2);
  uint32_t rec2 = stream->getPosition<uint32_t>(INDEX_REC2);
  uint32_t pathlen = stream->getPosition<uint32_t>(INDEX_PATH_LENGTH);

  char buffer[1024];
  sprintf(buffer,
          "% 10s : % 16llu % 16X\n"
          "% 10s : % 16llu % 16X\n"
          "% 10s : % 16llu % 16X\n"
          "% 10s : % 16u % 16X\n"
          "% 10s : % 16u % 16X\n"
          "% 10s : % 16u % 16X\n"
          "% 10s : % 16u % 16X\n",
          "total len", totalLength, totalLength, "message id", messageId,
          messageId, "sen1", sen1, sen1, "rec1", rec1, rec1, "sen2", sen2, sen2,
          "rec2", rec2, rec2, "pathlen", pathlen, pathlen);

  return buffer;
}