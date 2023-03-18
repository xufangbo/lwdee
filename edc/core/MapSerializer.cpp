#include "MapSerializer.h"

int MapSerializer::totalSize(Map *map) {
  int size = 0;

  size += 4;  // count;
  for (auto i = map->begin(); i != map->end(); i++) {
    size += 2;                // word-size
    size += i->first.size();  // word value
    size += 4;                // word.cout
  }
  return size;
}

ByteSpan_ref MapSerializer::serailize(Map *map) {
  int totalsize = this->totalSize(map);
  ByteSpan_ref bytes = make_shared<ByteSpan>(totalsize);

  bytes->putInt32((int)map->size());
  for (auto i = map->begin(); i != map->end(); i++) {
    // word-size
    short size = i->first.size();
    bytes->put(size);
    // word-value
    string value = i->first;
    bytes->put(value);
    // word.cout
    bytes->putInt32(i->second);
  }

  return bytes;
}

Map_ref MapSerializer::deserailize(ByteSpan *bytes) {
  bytes->reset();

  Map_ref map = make_shared<Map>();
  int count = 0;
  bytes->readInt32(count);

  for (int i = 0; i < count; i++) {
    short size = 0;
    bytes->read(size);

    string word(size + 1, '\0');
    bytes->reads((Byte *)word.data(), size);

    int wordCount = 0;
    bytes->readInt32(wordCount);

    Pair t = make_pair(word, wordCount);

    map->insert(t);
  }
  return map;
}