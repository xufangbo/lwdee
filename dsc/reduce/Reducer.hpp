#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <memory>
#include <mutex>

typedef std::vector<std::string> Words;
typedef std::list<std::shared_ptr<Words>> MapQueue;

class Reducer {
 private:
  std::vector<MapQueue> maps;

 private:
  bool isFull();
  void reduce();
  std::mutex mut;

 public:
  Reducer();
  void accept(int mapIndex,std::shared_ptr<Words> words);
};