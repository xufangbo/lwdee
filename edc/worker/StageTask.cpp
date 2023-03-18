#include "StageTask.h"

#include <algorithm>
#include <fstream>
#include <sstream>

ByteSpan_ref StageTask::runTask(Partition *p) {
  
  PartitionInput *partition = static_cast<PartitionInput *>(p);

  // textFile
  auto lines = this->textFile(partition);

  // flatMap
  Strings_ref words = make_shared<Strings>();
  for (string &line : *lines) {
    auto rc = this->flatMap(line);
    for_each(rc->begin(), rc->end(),
             [&words](string &x) { words->push_back(x); });

    rc.reset();
  }
  lines.reset();

  // map
  Tuples tuples;
  for (string &word : *words) {
    tuples.push_back(this->map(word));
  }

  words.reset();

  // write ddo
  TuplesSerialzer serializer;
  ByteSpan_ref bytes = serializer.serailize(&tuples);
  // Tuples_ref tuples2 = serializer.deserailize(bytes.get());
  return bytes;
};

Strings_ref StageTask::textFile(PartitionInput *p) {
  Strings_ref lines = make_shared<Strings>();

  fstream f;
  f.open(p->fileName);

  string line;
  while (getline(f, line)) {
    lines->push_back(line);
  }

  f.close();

  return lines;
}

Strings_ref StageTask::flatMap(string line) {
  Strings_ref words = make_shared<Strings>();

  stringstream iss(line);
  string word;
  while (getline(iss, word, ' ')) {
    words->push_back(word);
  }
  return words;
}

Tuple StageTask::map(string word) { return make_tuple(word, 1); }