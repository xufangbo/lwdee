#pragma once

#include <iostream>
#include <map>

#include "MapDCO.h"

typedef std::shared_ptr<MapDCO> MapDCOPtr;
typedef std::pair<int, MapDCOPtr> MapDCOPair;
typedef std::map<int, MapDCOPtr> Maps;

class MapSchedule {
 private:
  static Maps maps;

 public:
  static MapDCO* get(int mapKey);
  static void add(int key, MapDCOPtr mapdco);
};

void regist_map_start_service();
void regist_map_invoke_service();