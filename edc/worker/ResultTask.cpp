#include "ResultTask.h"

#include <map>

DDO ResultTask::runTask(Partition *p) {
  map<string, int> maps;

  DDO ddo;
  ddo.id = DdoManager::generateId();
  // DdoManager::set(ddo.id, bytes);
  return ddo;
}