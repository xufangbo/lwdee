#include "Step1Task.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include "core/MapSerializer.h"
#include "lwdee/lwdee.h"

PartitionStep1 Step1Task::run(PartitionStep1 *partition) {
  this->partition = partition;
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
  this->groupByKey(&tuples);

  return *partition;
};

std::hash<string> str_hash;

void Step1Task::groupByKey(Tuples *tuples) {
  int outSplitNums = this->partition->outSplitNums;
  Map maps[outSplitNums];
  vector<DDO> ddos;

  for (Tuple t : *tuples) {
    string word = get<0>(t);
    int index = str_hash(word) % outSplitNums;

    maps[index][word]++;
  }

  for (int i = 0; i < outSplitNums; i++) {
    // write ddo
    ByteSpan_ref bytes = MapSerializer().serailize(maps + i);
    DDO ddo = lwdee::createDDO();
    ddo.write(bytes);

    partition->ddos.push_back(ddo);
  }
}

Strings_ref Step1Task::textFile(PartitionStep1 *p) {
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

Strings_ref Step1Task::flatMap(string line) {
  Strings_ref words = make_shared<Strings>();

  stringstream iss(line);
  string word;
  while (getline(iss, word, ' ')) {
    words->push_back(word);
  }
  return words;
}

Tuple Step1Task::map(string word) { return make_tuple(word, 1); }