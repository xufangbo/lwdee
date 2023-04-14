#include "UhconnRouter.h"
#include "libgo.h"
#include "UhconnWorkNode.h"
#include "UhconnConfig.h"
#include "UhconnSimpleDB.h"
#include "UhconnMsgParser.h"
#include "UhconnSimpleDB.h"
#include "UhconnConfig.h"
#include "UhconnProtocol.h"
#include <sstream>
#include <errno.h>

//#define DEBUGINFO

// UhconnTCPServer UhconnRouter::tcpMsgServer;
// UhconnTCPServer UhconnRouter::tcpDataServer;  

// int receive_block(int fd,  DdoDataId &ddoId);
int send_block(DdoBlockData *block, int fd);


int UhconnRouter::msgRxHandler(char* data, int fd, int len){
    UhconnProtocol parser;
    std::vector<std::string> messages = parser.parseMsgBuff(data, len);
    for(int i=0;i<messages.size();i++)
    {
        UhconnMsgParser parser(messages[i]);
        if( parser.parse() < 0 ) {
            std::cout << "parse message failed!!!" << std::endl;
            return -1;
        }
        UhconnMessage msg;
        parser.getMsg(msg);
        #ifdef DEBUGINFO
        std::cout<<"unserialize msg:  "<<"msgcmd:"<<msg.getCmd()<<" type:"<<msg.getType()<<" method:"<<msg.getMethodName()<<
        " param:"<<msg.getMethodPara() <<std::endl;
        #endif
        getWorkNode()->inputMessage(msg);
    }

    return 0;
}

int UhconnRouter::dataRxHandler(char* data, int fd, int len){
    //std::string rsStr((char *)p2);
    UhconnProtocol parser;
    std::vector<std::string> messages = parser.parseMsgBuff(data, len);
    // int const txLen = ROUTER_MAX_DATA_PKT_LEN;
    for(int i=0;i<messages.size();i++){
        std::stringstream ss(messages[i]);
        std::string cmd;
        DdoDataId did;
        ss >> cmd;
        ss >> did;
        #ifdef DEBUGINFO
        std::cout<< "dataRxHandler process cmd: " <<cmd<<" : "<<did<<std::endl;
        #endif
        // DdoBlockData block;
        if(cmd.compare("pullDdoData")==0){
            // int ret = UhconnSimpleDB::getInstance().loadBlockFromLocal(did,block);
            DdoBlockData* block = UhconnSimpleDB::getInstance().getBlockFromLocal(did);
            uint32_t total_bytes = 0;
            char p[8] = {0}; 
            if( block == nullptr ){
                #ifdef DEBUGINFO
                std::cout<< "dataRxHandler err: block.len= "<<block.len <<" ret="<<ret<<std::endl;
                #endif
                int * ptype = (int*)&p[0];
                *ptype = htonl(ROUTER_DATA_NOT_FOUND);
                unsigned int * plen = (unsigned int*)&p[4];
                *plen = htonl(0);
                bool rtn = tcpDataServer.Send(p,8,fd);
                assert(rtn);
                return -1;
            }else{
                #ifdef DEBUGINFO
                std::cout<< "dataRxHandler send: " <<did<<" : "<<block.len<<std::endl;
                #endif
                int sendBytes = send_block(block, fd);
                assert(sendBytes >= 0);
             }
            #ifdef DEBUGINFO
            std::cout<<"dataRxHandler send len="<< ret << std::endl;
            #endif
        }else if(cmd.compare("delDdoData")==0){
            UhconnSimpleDB::getInstance().deleteBlock(did);
        }else if(cmd.compare("pushDdoData")==0){
            if(UhconnSimpleDB::getInstance().isBlockAtLocal(did) || UhconnSimpleDB::getInstance().isBlockPreparing(did)){
                char res_cmd[] = "exist";
                bool ret = tcpDataServer.Send(res_cmd, strlen(res_cmd), fd);
                assert(ret);
            }else{
                char res_cmd[] = "requestData";
                if(tcpDataServer.Send(res_cmd, strlen(res_cmd),fd)){
                    // int ret = receive_block(fd, did);
                    // assert(ret >= 0);
                }  
            }
        }

    }

    
    return 0;
}


#if 0
int dataRxHandler2(void* p1, void* p2, int fd, int len){
    UhconnRouter *router = (UhconnRouter *)p1;
    UhconnProtocol parser;
    std::vector<std::string> messages = parser.parseMsgBuff((char *)p2, len);
    int ret=0;
    for(int i=0;i<messages.size();i++){
        std::stringstream ss(messages[i]);
        std::string cmd;
        DdoDataId did;
        ss >> cmd;
        ss >> did;
        #ifdef DEBUGINFO
        std::cout<< "dataRxHandler process cmd: " <<cmd<<" : "<<did<<std::endl;
        #endif
        DdoBlockData *block;
        if(cmd.compare("pullDdoData")==0){
            block = UhconnSimpleDB::getInstance().getBlockFromLocal(did);
            uint32_t total_bytes = 0;
            char p[8] = {0}; 
            if(block == nullptr){
                #ifdef DEBUGINFO
                std::cout<< "dataRxHandler err -1!!"<<std::endl;
                #endif
                int * ptype = (int*)&p[0];
                *ptype = htonl(ROUTER_DATA_NOT_FOUND);
                unsigned int * plen = (unsigned int*)&p[4];
                *plen = htonl(0);
                ret = router->tcpDataServer.Send(p,8,fd);
            }else{
                #ifdef DEBUGINFO
                std::cout<< "dataRxHandler send: did= " <<did<<" : len="<<block->len<<std::endl;
                #endif
                ret = send_block(block,fd,total_bytes);
                if(ret >=0){
                    UhconnSimpleDB::getInstance().releaseBlock(did);
                }  
                #ifdef DEBUGINFO
                std::cout<< "dataRxHandler send "<<total_bytes<<" for did " <<did<<std::endl;
                #endif
            }
        }else if(cmd.compare("delDdoData")==0){
            UhconnSimpleDB::getInstance().deleteBlock(did);
            #ifdef DEBUGINFO
            std::cout<<"del ddo "<<did<<" from remote cmd!"<<std::endl;
            #endif
            ret = 0;
        }else if(cmd.compare("pushDdoData")==0){
            if(UhconnSimpleDB::getInstance().isBlockAtLocal(did) || UhconnSimpleDB::getInstance().isBlockPreparing(did)){
                char res_cmd[] = "exist";
                ret = router->tcpDataServer.Send(res_cmd, strlen(res_cmd), fd);
            }else{
                char res_cmd[] = "requestData";
                ret = router->tcpDataServer.Send(res_cmd, strlen(res_cmd),fd);
                if(ret>=0){
                    ret = receive_block(fd, did);
                }  
            }
        }
    }
    return ret;
}
#endif

UhconnRouter::UhconnRouter(UhconnWorkNode* node) : 
    tcpMsgServer(UhconnConfig::getInstance().getMsgPort(), bind(&UhconnRouter::msgRxHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)),
    tcpDataServer(UhconnConfig::getInstance().getDataPort(), bind(&UhconnRouter::dataRxHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
{
    workNode = node;
}

UhconnRouter::~UhconnRouter()
{
    tcpMsgServer.detach();
    tcpDataServer.detach();
}

int UhconnRouter::regNode(int id, const NodeInfo& info) {
    auto it = nodeMap.find(id);
    if (it != nodeMap.end()) {
        it->second = info;
    } else {
        nodeMap.insert(make_pair(id, info));
    }
    return 0;
}

int UhconnRouter::setupRouteInfoFromConf(void){
    UhconnConfig::getInstance().getRouteInfo(nodeMap);
    return 0;
}

// int UhconnRouter::nodeInfo(int id,NodeInfo& info){
//     map<int, NodeInfo>::iterator it = nodeMap.find(id);
//     if(it != nodeMap.end()) {
//         info.ipv4Addr = it->second.ipv4Addr;
//         info.msgPort = it->second.msgPort;
//         info.dataPort = it->second.dataPort;
//         info.nodeId = it->second.nodeId;
//         info.dataClient = it->second.dataClient;
//         info.msgClient = it->second.msgClient;
//         info.dataLock = it->second.dataLock;
//         info.msgLock = it->second.msgLock;
//         return 0;
//     }
//     return -1;
// }
int UhconnRouter::nodeInfo(int id, NodeInfo& info) {
    auto it = nodeMap.find(id);
    if (it != nodeMap.end()) {
        info = it->second;
        return 0;
    }
    return -1;
}

void printNodeInfo(const NodeInfo& info) {
    std::cout << "NodeInfo:\n";
    std::cout << "ipv4Addr: " << info.ipv4Addr << "\n";
    std::cout << "nodeId: " << info.nodeId << "\n";
    std::cout << "msgPort: " << info.msgPort << "\n";
    std::cout << "dataPort: " << info.dataPort << "\n";
    std::cout << "msgClient: " << info.msgClient << "\n";
    std::cout << "dataClient: " << info.dataClient << "\n";
    std::cout << "msgLock: " << info.msgLock << "\n";
    std::cout << "dataLock: " << info.dataLock << "\n";    
}

NodeInfo& UhconnRouter::getNodeInfo(int destNodeId) {
    static NodeInfo dummy;
    dummy.nodeId = -1;

    co_mutex_lock lock(mapLock);

    auto it = nodeMap.find(destNodeId);
    if (it == nodeMap.end()) {
        return dummy;
    }

    NodeInfo& node = it->second;

    if (node.msgClient == nullptr) {
        node.msgClient =new UhconnTCPClient();
        assert(node.msgClient);
        node.msgClient->setup(node.ipv4Addr, node.msgPort);
    }
    if( node.dataClient == nullptr) {
        node.dataClient = new UhconnTCPClient();
        assert(node.dataClient);
        node.dataClient->setup(node.ipv4Addr, node.dataPort);
    }
    return node;
}

int UhconnRouter::sendMsg(UhconnMessage &msg) {
    int destNodeId = msg.getDestNodeId();
    int localNodeId = workNode->itsAddr();
    if(localNodeId == destNodeId){
        workNode->inputMessage(msg);
        return 0;
    }

    NodeInfo& node = getNodeInfo(destNodeId);
    if (node.nodeId == -1) {
        std::cout << "UhconnRouter::sendMsg failed to find destination node: " << destNodeId;
        return -1;
    }
    UhconnTCPClient *client = node.msgClient;
    if( client == nullptr ) {
        return -3;
    }
    co_mutex_lock lock(*(node.msgLock));

    string str = UhconnProtocol::left_enc + msg.getMessageJsonStr()+ UhconnProtocol::right_enc;

    if(!client->Send(str)) {
        cerr << "Failed to send message." << endl;
        node.msgClient->exit();
        delete node.msgClient;
        node.msgClient = nullptr;
        return -2;            
    }
    return 0;

}

// int UhconnRouter::sendMsg(UhconnMessage& msg){
//     int ret = 0;
//     int rd_chan;
//     NodeInfo destInfo;

//     int localNodeId = workNode->itsAddr();
//     if(localNodeId == msg.getDestNodeId()){
//         workNode->inputMessage(msg);
//         return 0;
//     }
//     ret = nodeInfo(msg.getDestNodeId(), destInfo);
//     if(ret < 0){
//         cout << "sendMsg:dest work node not found!!" << endl;
//         return -1;
//     }

//     printNodeInfo(destInfo);    
//     destInfo.msgLock->lock();
//     if(destInfo.msgClient == NULL){
//         destInfo.msgClient =new UhconnTCPClient();
//         destInfo.msgClient->setup(destInfo.ipv4Addr, destInfo.msgPort);
//         regNode(msg.getDestNodeId(),destInfo);
//     }
//     destInfo.msgLock->unlock();
//     string str = UhconnProtocol::left_enc + msg.getMessageJsonStr()+ UhconnProtocol::right_enc ;
//     #ifdef DEBUGINFO
//     std::cout << "sendMsg:"<< str << std::endl;
//     #endif
//     destInfo.msgLock->lock();
//     ret = destInfo.msgClient->Send(str);
//     if(ret <0){
//         destInfo.msgClient->exit();
//         delete destInfo.msgClient;
//         destInfo.msgClient = NULL;
//         ret = -2;
//         std::cerr << "Error: !!!!!!!estInfo.msgClient == NULL." << std::endl;
//         std::abort();
//     }
//     destInfo.msgLock->unlock();
//     return ret;
//  }


void cleanTcpClient(UhconnTCPClient** pclient) {
    assert(pclient);
    UhconnTCPClient *client = *pclient;
    assert(client);
    client->exit();
    delete client;
    client = nullptr;    
}

/**
 * @brief 拉取远端ddo到本地
 * 
 * @param destNodeId ddo所在worknode
 * @param ddoId ddo标识
 * @return int 
 */
 int UhconnRouter::pullData(int destNodeId, DdoDataId ddoId){
    if(UhconnSimpleDB::getInstance().isBlockAtLocal(ddoId)){
        return ok;
    }

    if(workNode->itsAddr() == destNodeId){
        return ok;
    }

    NodeInfo& destInfo = getNodeInfo(destNodeId);
    if (destInfo.nodeId == -1) {
        std::cout << "UhconnRouter::pullData failed to find destination node: " << destNodeId;
        return err_node;
    }
    UhconnTCPClient *client = destInfo.dataClient;
    if( client == nullptr ) {
        return err_client;
    }

    std::stringstream ss;
    ss<<"pullDdoData"<<" "<<ddoId;
    string str = UhconnProtocol::left_enc + ss.str()+ UhconnProtocol::right_enc;

    co_mutex_lock lock(*(destInfo.dataLock));


    if(!client->Send(str)) {
        cerr << "Failed to send message." << endl;
        cleanTcpClient(&client);
        return err_send;            
    }

    return receive_block(client, ddoId);
 }

 int UhconnRouter::pullData(DdoDataId ddoId){
    int destNodeId = (ddoId>>48) & 0xffff;
    return pullData(destNodeId, ddoId);
 }


int UhconnRouter::receive_block(UhconnTCPClient* client, DdoDataId ddoId){

    //先获取数据长度
    unsigned char head[8];
    int ret = client->receive(head,sizeof(head));
    if( ret < 0 ) {
        cerr << "Failed to receive data." << endl;
        cleanTcpClient(&client);
        return err_rcv;
    }
    // DdoBlockData block;

    int type = ntohl(*((int*)&head[0]));
    if( type == ROUTER_DATA_NOT_FOUND ) {
        return err_nodata;
    }
    int len = ntohl(*((int*)&head[4]));
    if( len <= 0 ) {
        return err_datalen;
    }

    DdoBlockData* block = UhconnSimpleDB::getInstance().createBlock(ddoId, len, 0);
    #ifdef DEBUGINFO
    std::cout << "tcp will rcv len:" << block.len << std::endl;
    #endif

    uint64_t totleBytes = 0;
    while( totleBytes < block->len ) {
        ret = client->receive((unsigned char*)block->data + totleBytes, block->len-totleBytes);
        if( ret > 0 ) {
            totleBytes += ret;
        }
        else if( ret == 0 ) {
            break;
        }
        else {
            std::cout << "TCP receive error!!! at bytes: " << totleBytes << std::endl;
            free(block->data);
            block->data = nullptr;
            cleanTcpClient(&client);
            return err_rcv;
        }
    }
    return ok;
}

//  int UhconnRouter::pullData2(DdoDataId ddoId){
//     int destNodeId = ddoId>>48 & 0xffff;
//     int ret = -100;
//     ret = pullData2(destNodeId, ddoId);
//     if(ret >=0){
//         return ret;
//     }else{
//         std::cout<<"pulldata2 ret "<<ret<<" node:id = "<<destNodeId<<" : "<<ddoId<<std::endl;
//     }
//     int nodeCnt = 0;
//     for(auto it : nodeMap){
//         if(++nodeCnt > UhconnConfig::getInstance().getNodeAmount()){
//             break;
//         }
//         if(it.first == destNodeId){
//             std::cout<<"it.first == destNodeId continue"<<it.first<<":"<<destNodeId<<" "<<std::endl;
//             continue;
//         }
//         std::cout<<"try to pull data from node "<<it.first<<std::endl;
//         ret = pullData2(it.first, ddoId);
//         if(ret >= 0){
//             std::cout<<"at last get data from node "<<it.first<<std::endl;
//             break;
//         }
//     }
//     return ret;
//  }


int UhconnRouter::pushData(int destNodeId, DdoDataId ddoId){

    int localNodeId = workNode->itsAddr();
    if(localNodeId == destNodeId){
        return 0;
    }
    NodeInfo& destInfo = getNodeInfo(destNodeId);
    if (destInfo.nodeId == -1) {
        std::cout << "UhconnRouter::sendMsg failed to find destination node: " << destNodeId;
        return -1;
    }
    UhconnTCPClient *client = destInfo.dataClient;
    if( client == nullptr ) {
        return -3;
    }    

    #ifdef DEBUGINFO
    std::cout<<"push ddo "<<ddoId <<" to node "<<destNodeId <<std::endl;
    #endif 
    std::stringstream ss;
    ss<<"pushDdoData"<<" "<<ddoId;
    std::string sendStr= UhconnProtocol::left_enc + ss.str() +UhconnProtocol::right_enc;
    co_mutex_lock lock(*(destInfo.dataLock));
    if(!client->Send(sendStr)) {
        cerr << "Failed to send data ." << ss.str() << endl;
        client->exit();
        delete client;
        client = nullptr;
        return -2;            
    }
    char rxBuf[64];
    memset(rxBuf,0,sizeof(rxBuf));
    int ret = client->receive((unsigned char*)rxBuf,sizeof(rxBuf));
    if(ret <= 0){
        return -5;
    }
    if(strcmp(rxBuf,"requestData")==0){
        DdoBlockData *block = UhconnSimpleDB::getInstance().getBlockFromLocal(ddoId);
        if(!block){
            return -6; 
        }
        #ifdef DEBUGINFO
        std::cout<<"peer server resp with request-data, start send"<<std::endl;
        #endif
        ret = send_block(block, client->getSockFd());
        assert(ret >= 0);
        UhconnSimpleDB::getInstance().releaseBlock(ddoId);
    }else{
        ret = 0;
    }


    return ret;
 }

int UhconnRouter::deleteData(int destNodeId, DdoDataId ddoId){

    int localNodeId = workNode->itsAddr();
    if(localNodeId == destNodeId){
        return -1;
    }
    NodeInfo& destInfo = getNodeInfo(destNodeId);
    if (destInfo.nodeId == -1) {
        std::cout << "UhconnRouter::sendMsg failed to find destination node: " << destNodeId;
        return -1;
    }

    UhconnTCPClient *client = destInfo.dataClient;
    if( client == nullptr ) {
        return -3;
    }
    std::stringstream ss;
    ss<<"delDdoData"<<" "<<ddoId;
    std::string sendStr= UhconnProtocol::left_enc + ss.str() +UhconnProtocol::right_enc;
    co_mutex_lock lock(*(destInfo.dataLock));
    if(!client->Send(sendStr)) {
        cerr << "Failed to send data ." << ss.str() << endl;
        client->exit();
        delete client;
        client = nullptr;
        return -2;            
    }

    return 0;
 }

int UhconnRouter::deleteData(DdoDataId ddoId){
    int nodeCnt = 0;
    for(auto it : nodeMap){
        if(++nodeCnt > UhconnConfig::getInstance().getNodeAmount()){
            break;
        }
        deleteData(it.first, ddoId);
    }
    return 0;
 }

UhconnWorkNode * UhconnRouter::getWorkNode(){
    return workNode;
  }



int send_block(DdoBlockData *block, int fd){
    int total_bytes = 0;
    char p[8] = {0}; 
    int * ptype = (int*)&p[0];
    *ptype = htonl(block->type);
    int * plen = (int*)&p[4];
    *plen = htonl(block->len);
    total_bytes = UhconnTCPServer::Send(p,8,fd);
    if(total_bytes < 0) {
        return total_bytes;
    }            
    total_bytes = UhconnTCPServer::Send((char*)block->data, block->len, fd);
    return total_bytes;
}