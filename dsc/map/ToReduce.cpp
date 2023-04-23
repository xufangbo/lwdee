#include "ToReduce.hpp"

#include "antelope/LaneClient.hpp"
#include "protocal/TcpRequest.hpp"
#include "core/DscConfig.hpp"
#include "core/NodeConfig.hpp"
#include "core/cjson.hpp"
#include "core/log.hpp"

void ToReduce::create_dcos(PartitionMap* input) {
  this->input = input;

  for (auto& reduceVoxorId : input->reduceVoxors) {
    auto dco = NodeConfig::voxorId(reduceVoxorId);
    reduceDcos.push_back(dco);
  }
}

void ToReduce::send(vector<ReduceRecord>* words) {
  int split = reduceDcos.size();

  auto reduceWords = std::make_shared<vector<vector<ReduceRecord>>>(split);
  for (int i = 0; i < words->size(); i++) {
    ReduceRecord& word = words->at(i);
    int x = _hash(word.did) % split;
    reduceWords->at(x).push_back(word);
  }

  for (int i = 0; i < split; i++) {
    VoxorId voxorId = this->reduceDcos[i];
    TNode* node = NodeConfig::byNodeId(voxorId.nodeId);

    auto client = LaneClient::create(node->ip.c_str(), node->port);
    RequestCallback callback = [](BufferStream* inputStream) {
      auto len = inputStream->get<uint32_t>();
      auto content = inputStream->getString(len);
      // logger_debug("recive(%d) :  %s", len, content.c_str());
    };

    auto json = ReduceInvokeData(input->index, i, &reduceWords->at(i)).toJson();
    client->invoke(ServicePaths::reduce_invoke, (void*)json.c_str(), json.size(), callback);
    // auto waitTime = client->wait();
    // logger_trace("wait reduce time %lf", waitTime);
    logger_trace("%d / %d send to reduce %s:%d %d records",i,split,node->name.c_str(),voxorId.voxorKey,reduceWords->at(i).size());
  }
}