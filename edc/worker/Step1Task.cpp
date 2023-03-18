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
  auto words = this->flatMaps(lines);
  lines.reset();

  // map
  auto tuples = this->maps(words);
  words.reset();

  // write ddo
  this->groupByKey(tuples.get());
  tuples.reset();

  return *partition;
};

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

Strings_ref Step1Task::flatMaps(Strings_ref lines) {
  Strings_ref words = make_shared<Strings>();
  for (string &line : *lines) {
    auto rc = this->flatMap(line);
    for_each(rc->begin(), rc->end(),
             [&words](string &x) { words->push_back(x); });

    rc.reset();
  }

  return words;
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

Tuples_ref Step1Task::maps(Strings_ref words) {
  auto tuples = make_shared<Tuples>();
  for (string &word : *words) {
    tuples->push_back(this->map(word));
  }
  return tuples;
}

Tuple Step1Task::map(string word) { return make_tuple(word, 1); }

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