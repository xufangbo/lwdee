#include "StageTask.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include "core/TuplesSerialzer.h"

void StageTask::runTask(Partition *p) {
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
  for_each(words->begin(), words->end(),
           [&tuples, this](string &x) { tuples.push_back(this->map(x)); });

  words.reset();

  // write ddo
  ByteSpan_ref bytes = TuplesSerialzer().serailize(&tuples);
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