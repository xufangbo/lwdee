#include "TuplesSerialzer.h"

int TuplesSerialzer::totalSize(Tuples *tuples) {
  int size = 0;

  size += 4;  // count;
  for (Tuple &t : *tuples) {
    size += 2;
    size += get<0>(t).size();
    size += 4;
  }
}

ByteSpan_ref TuplesSerialzer::serailize(Tuples *tuples) {
  int totalsize = this->totalSize(tuples);
  ByteSpan_ref bytes = make_shared<ByteSpan>(totalsize);

  bytes->putInt32((int)tuples->size());
  for (Tuple &t : *tuples) {
    // word-size
    bytes->put((short)get<0>(t).size());
    // word-value
    bytes->put(get<0>(t));
    // count
    bytes->putInt32(get<1>(t));
  }

  return bytes;
}

Tuples_ref TuplesSerialzer::deserailize(ByteSpan *bytes) {
    
  Tuples_ref tuples = make_shared<Tuples>();
  int count = 0;
  bytes->readInt32(count);

  for (int i = 0; i < 10; i++) {
    short size;
    bytes->read(size);

    string word(size + 1, '\0');
    bytes->reads(word.data(), size);

    int wordCount = 0;
    bytes->readInt32(wordCount);

    Tuple t = make_tuple(word, wordCount);

    tuples->push_back(t);
  }
  return tuples;
}