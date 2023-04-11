#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <mutex>

typedef std::vector<std::string> Words;
typedef std::list<Words> MapQueue;

class Reducer {
 private:
  std::vector<MapQueue> maps;

 private:
  bool isFull();
  void reduce();
  std::mutex mut;

 public:
  Reducer();
  void accept(int mapIndex, Words& words);
};