#include "UhconnDco.h"
#include "core/UhconnVoxor.h"
#include "core/UhconnVoxorFactory.h"
#include "core/UhconnSimpleDB.h"
#include <sstream>
#include "core/UhconnWorkNode.h"
#include "api/UhconnDdo.h"
#include "log.h"

#ifdef RUN_DEMO

#undef logDebug
#undef logInfo
// #define logDebug printf
// #define logInfo printf
#define logDebug 
#define logInfo 
#endif

std::string UhconnDco::wait_ddo(std::string ddo_id) {
    std::stringstream ss(ddo_id);
    DdoDataId ddoId;
    int nodeId;
    int post;
    ss >> ddoId >> nodeId >> post;
    logInfo("receive sb check ddo data:%llu", ddoId);
    DdoBlockData data;
    while(true) {
        //int ret = UhconnSimpleDB::getInstance().loadBlockFromLocal(ddoId,data);
        bool ret = UhconnSimpleDB::getInstance().isBlockAtLocal(ddoId);
        if( !ret ) {
            co_yield;
        }
        else {
            logInfo("yes! ddo data:%llu has been ready", ddoId);
            if(post){
               UhconnWorkNode* localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode(); 
               localNode->itsRouter().pushData(nodeId,ddoId);
            }
            return ddo_id;
        }
    }
}

UhconnDco::UhconnDco()
{
    itsVoxor = UhconnVoxorFactory::getInstance().createVoxor(this);
    df = new UhconnDcoRef(itsVoxor->getAddr());
    getFunctionTable()["wait"] = &UhconnDco::wait_ddo;
}

UhconnDco::~UhconnDco()
{

}

std::string& UhconnDco::itsAddr(void) {
    return itsVoxor->getAddr();
}

co_chan<UhconnMessage> UhconnDco::getMsgQ(void) {
    return itsVoxor->getMsgQ();
}

UhconnDcoRef UhconnDco::create(std::string className, int in_destNode) {
    //构建create消息
    UhconnWorkNode* localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();

    int destNode = localNode->itsScheduler().getDestWorkNode();
    if(in_destNode != -1 ) {
        destNode = in_destNode;
    }
    UhconnMessage msg(localNode->itsAddr(), destNode, MSG_CMD_CREATE, MSG_TYPE_REQ);
    msg.setMethodName("create");
    msg.setMethodPara(className);
    UhconnSimpleAddr addr(destNode, 0);
    // std::cout << "voxor addr: " << addr.itsValue() << std::endl;
    msg.setDestVoxor(addr.itsValue());
    msg.setSrcVoxor(itsAddr());

    auto channel = localNode->addToWaitingTable(msg);
    printf("%s:%d %s get msgid: %lld",__FILE__,__LINE__,__FUNCTION__,msg.getMsgId());
    assert(localNode->getFromWaitingTable(msg));

    logDebug("request create class:%s at worknode:%d",className.c_str(), msg.getDestNodeId());
    localNode->itsRouter().sendMsg(msg);

    //阻塞等待
    UhconnMessage result;
    channel->pop(result);
    localNode->removeFromWaitingTable(msg);
    // std::cout << "UhconnDcoRef:" << result.getCmd() << result.getMethodPara() << std::endl;
    UhconnDcoRef dco(result.getMethodPara());
    logDebug("create Dco:%s at dco:%s success!", dco.itsBelongVoxorId().c_str(), dco.itsBelongVoxorId().c_str());
    return dco;
}

UhconnDcoRef UhconnDco::create(std::string className, std::string classConstructor) {
    UhconnWorkNode* localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    int destNode = localNode->itsScheduler().getDestWorkNode();
    UhconnMessage msg(localNode->itsAddr(), destNode, MSG_CMD_CREATE, MSG_TYPE_REQ);
    msg.setMethodName("create");
    msg.setMethodPara(className + " " + classConstructor);
    localNode->itsRouter().sendMsg(msg);
    UhconnMessage result;
    getMsgQ() >> result;
    // std::cout << "UhconnDcoRef:" << result.getCmd() << result.getMethodPara() << std::endl;
    UhconnDcoRef dco(result.getMethodPara());
    return dco;

}

/**
 * @brief 远程调用异步请求
 * @details 请求参数直接在消息中，适用简单的参数
 * @param dco 远程被请求端的描述信息
 * @param funcName 远程方法名
 * @param funcPara 远程方法执行参数
 * @return const UhconnDdoRef 远程调用结果的存储信息
 */
const UhconnDdoRef UhconnDco::async(UhconnDcoRef& dco, std::string funcName, std::string funcPara) {
    UhconnMessage msg;
    UhconnDdoRef ddoref(dco.itsBelongVoxorId());
    msg.setupMessage(itsWorkNodeSn(), dco.itsBelongWorkNodeId(), MSG_CMD_ASYNC, MSG_TYPE_REQ);
    msg.setMethodName(funcName);
    msg.setMethodPara(funcPara);
    msg.setDestVoxor(dco.itsBelongVoxorId());
    msg.setSrcVoxor(itsAddr());
    msg.setDdoId(ddoref.itsId());
    UhconnVoxorFactory::getInstance().getLocalWorkNode()->itsRouter().sendMsg(msg);

    return ddoref;    
}

/**
 * @brief 远程调用异步请求
 * @details 请求参数用ddo传输 适用大块的参数传递
 * @param dco 
 * @param reqFuncName 
 * @param reqPara 
 * @return UhconnDdoRef 
 */
UhconnDdoRef UhconnDco::async(UhconnDcoRef& dco, std::string reqFuncName, UhconnDdo* reqPara) {
    UhconnDdoRef ddoref(dco.itsBelongVoxorId());
    UhconnMessage msg(itsWorkNodeSn(), dco.itsBelongWorkNodeId(), MSG_CMD_ASYNC, MSG_TYPE_REQ);
    msg.setSrcVoxor(itsAddr());
    msg.setDestVoxor(dco.itsBelongVoxorId());
    msg.setMethodName(reqFuncName);
    msg.setMethodPara(reqPara->toJsonString());
    msg.setDdoId(ddoref.itsId());
    logDebug("request remote dco:%s exec function:%s with para ddo:%llu, return in:%llu", 
                msg.getDestVoxor().c_str(), 
                msg.getMethodName().c_str(), 
                reqPara->itsRef().itsId(), 
                ddoref.itsId());
    UhconnVoxorFactory::getInstance().getLocalWorkNode()->itsRouter().sendMsg(msg);
    std::cout << "async req msg:" << msg.getMethodPara() << std::endl;
    return ddoref;
}


UhconnDdo* UhconnDco::wait(UhconnDdoRef& ddo_ref) {
    // 先判断本地是否已经有预备块或本地块了
    if (UhconnSimpleDB::getInstance().isBlockPreparing(ddo_ref.itsId()) || UhconnSimpleDB::getInstance().isBlockAtLocal(ddo_ref.itsId())) {
        return new UhconnDdo(ddo_ref);
    }

    // 构造消息
    UhconnWorkNode* localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
    UhconnSimpleAddr addr(ddo_ref.itsVoxorId());
    UhconnMessage msg;
    msg.setupMessage(localNode->itsAddr(), addr.workNodeSn(), MSG_CMD_WAIT, MSG_TYPE_REQ);
    msg.setMethodName("wait");
    std::stringstream ss;
    ss << ddo_ref.itsId() << " " << addr.workNodeSn() <<" "<< 0; // don't post
    msg.setMethodPara(ss.str());
    msg.setDestVoxor(ddo_ref.itsVoxorId());
    msg.setSrcVoxor(itsAddr());

    auto channel = localNode->addToWaitingTable(msg);
    printf("%s:%d %s wait.getMsgId: %lld",__FILE__,__LINE__,__FUNCTION__,msg.getMsgId());
    assert(localNode->getFromWaitingTable(msg));
    logDebug("ask remote dco:%s if the result data:%llu have been ready", msg.getDestVoxor().c_str(), ddo_ref.itsId());
    localNode->itsRouter().sendMsg(msg);

    // 阻塞等待response
    UhconnMessage result;
    channel->pop(result);
    localNode->removeFromWaitingTable(msg);
    logDebug("remote dco answer back: the ddo data:%s is ready", result.getMethodPara().c_str());
    return new UhconnDdo(ddo_ref, true);
    // if (channel->TimedPop(result, std::chrono::seconds(300))) {
    //     localNode->removeFromWaitingTable(msg);
    //     logDebug("remote dco answer back: the ddo data:%s is ready", result.getMethodPara().c_str());
    //     return new UhconnDdo(ddo_ref, true);
    // } else {
    //     localNode->removeFromWaitingTable(msg);
    //     logDebug("waiting for ddo data:%llu timeout", ddo_ref.itsId());
    //     return nullptr;
    // }
}


// std::vector<UhconnDdo*> UhconnDco::wait(std::vector<UhconnDdoRef>& ddo_ref, bool postIfReady) {
//     UhconnMessage msg;
//     int size = ddo_ref.size();
//     int wait_count = 0;
//     std::vector<UhconnDdo*> result(size);
//     for(int i=0; i<size; i++){
//         result[i] = new UhconnDdo(ddo_ref[i]); 
//         if(UhconnSimpleDB::getInstance().isBlockPreparing(ddo_ref[i].itsId()) ||UhconnSimpleDB::getInstance().isBlockAtLocal(ddo_ref[i].itsId())){
//             continue;  
//         }
//         UhconnWorkNode* localNode = UhconnVoxorFactory::getInstance().getLocalWorkNode();
//         UhconnSimpleAddr addr(ddo_ref[i].itsVoxorId());
//         msg.setupMessage(localNode->itsAddr(), addr.workNodeSn(), MSG_CMD_WAIT, MSG_TYPE_REQ);
//         msg.setMethodName("wait");
//         std::stringstream ss;
//         ss << ddo_ref[i].itsId() << " " << localNode->itsAddr() <<" "<< postIfReady?1:0; // don't post
//         msg.setMethodPara(ss.str());
//         msg.setDestVoxor(ddo_ref[i].itsVoxorId());
//         msg.setSrcVoxor(itsAddr());
//         logDebug("ask remote dco:%s if the result data:%llu have been ready", msg.getDestVoxor().c_str(), ddo_ref[i].itsId());
//         auto channel = localNode->addToWaitingTable(msg);

//         localNode->itsRouter().sendMsg(msg);
//         wait_count++;
//     }
    
//     // 阻塞等待response
//     UhconnMessage res_msg;
//     for(int i=0;i<wait_count;i++){
//         getMsgQ() >> res_msg; //@todo 需要增加超
//     }
//     logDebug("all remote dco answer back: the ddo list is ready");
//     std::cout<<"all remote dco answer back: the ddo list is ready"<<std::endl;
//     return result;    
    
// }

std::vector<UhconnDdo*> UhconnDco::wait(std::vector<UhconnDdoRef>& ddo_refs, bool postIfReady) {
    UhconnSimpleDB& db = UhconnSimpleDB::getInstance();
    UhconnVoxorFactory& vf = UhconnVoxorFactory::getInstance();
    UhconnWorkNode* localNode = vf.getLocalWorkNode();

    std::vector<UhconnMessage> msgs;
    std::vector<UhconnDdo*> results;

    for (auto& ddo_ref : ddo_refs) {
        if (db.isBlockPreparing(ddo_ref.itsId()) || db.isBlockAtLocal(ddo_ref.itsId())) {
            results.emplace_back(new UhconnDdo(ddo_ref, true));
            continue;
        }

        UhconnSimpleAddr addr(ddo_ref.itsVoxorId());
        UhconnMessage msg;
        msg.setupMessage(localNode->itsAddr(), addr.workNodeSn(), MSG_CMD_WAIT, MSG_TYPE_REQ);
        msg.setMethodName("wait");
        std::stringstream ss;
        ss << ddo_ref.itsId() << " " << addr.workNodeSn() << " " << postIfReady?1:0; // don't post
        msg.setMethodPara(ss.str());
        msg.setDestVoxor(ddo_ref.itsVoxorId());
        msg.setSrcVoxor(itsAddr());
        auto channel = localNode->addToWaitingTable(msg);
        std::cout <<"++++wait.getMsgId:"<<msg.getMsgId()<<std::endl;

        logDebug("ask remote dco:%s if the result data:%llu have been ready", msg.getDestVoxor().c_str(), ddo_ref.itsId());
        localNode->itsRouter().sendMsg(msg);

        msgs.emplace_back(std::move(msg));
    }

    for (auto& msg : msgs) {
        auto channel = localNode->getFromWaitingTable(msg);
        if (!channel) {
            continue;
        }
        UhconnMessage result;
        channel->pop(result);
        localNode->removeFromWaitingTable(msg);
        logDebug("remote dco answer back: the ddo data:%s is ready", result.getMethodPara().c_str());
        results.emplace_back(new UhconnDdo(ddo_refs[&msg - msgs.data()], true));
    }

    return results;
}


int UhconnDco::itsWorkNodeSn(void) {
    return itsVoxor->itsBelongToWorkNode()->itId();
}