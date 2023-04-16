#include "MapSchedule.hpp"

#include "core/log.hpp"
#include "core/Exception.hpp"
#include "net/BufferStream.hpp"
#include "server/TcpResponse.hpp"

Maps MapSchedule::maps;

MapDCO* MapSchedule::get(int mapKey) {
  auto it = maps.find(mapKey);
  if (it == maps.end()) {
    throw Exception("can't find map dco by key " + std::to_string(mapKey),ZONE);
  }

  return it->second.get();
}
void MapSchedule::add(int key, MapDCOPtr mapdco) {
  MapDCOPair pair(key, mapdco);
  maps.insert(pair);
}

void regist_map_start_service() {
  TcpResponse::regist(ServicePaths::map_start, [](BufferStream* inputStream, BufferStream* outputStream) {
    // 1. inputStream -> order request parameter
    auto len = inputStream->get<uint32_t>();
    auto content = inputStream->getString(len);

    MapDCOPtr mapdco = std::make_shared<MapDCO>();
    int mapIndex = mapdco->start(content);

    MapSchedule::add(mapIndex, mapdco);

    // 3. outputStream
    std::string message = "succeed";
    outputStream->put<uint32_t>(message.size());
    outputStream->put(message);
  });
}

void regist_map_invoke_service() {
  TcpResponse::regist(ServicePaths::map_invoke, [](BufferStream* inputStream, BufferStream* outputStream) {
    // 1. inputStream -> order request parameter
    auto len = inputStream->get<uint32_t>();
    auto content = inputStream->getString(len);

    auto lines = std::make_shared<vector<MapRecord>>();
    MapInvokeData mapInvokeDta(0,0, lines.get());
    mapInvokeDta.fromJson(&content);

    auto mapdco = MapSchedule::get(mapInvokeDta.mapIndex);
    if(mapdco==nullptr){
      logger_error("mapdco is null");
    }
    mapdco->map(lines);

    // 3. outputStream
    std::string message = "succeed";
    outputStream->put<uint32_t>(message.size());
    outputStream->put(message);

    // logger_debug("execute map.invoke service");
  });
}
