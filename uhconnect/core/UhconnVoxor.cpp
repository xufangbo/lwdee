#include "UhconnVoxor.h"
#include "UhconnWorkNode.h"
#include "UhconnVoxorFactory.h"
#include "api/UhconnDco.h"
#include "core/UhconnSimpleDB.h"
#include "UhconnRouter.h"
#include "api/UhconnDdo.h"
#include "api/UhconnDdoRef.h"

UhconnVoxor::UhconnVoxor() : ItsAddr(0, 0)
{

}


UhconnVoxor::UhconnVoxor(UhconnWorkNode* worknode, UhconnDco* dco, int sn) : ItsAddr(worknode->itsAddr(), sn) ,MsgQ(10)
{
    belongTo = worknode;
    ItsDco = dco;
}

UhconnVoxor::~UhconnVoxor()
{

}

std::string& UhconnVoxor::getAddr(void) {
    return ItsAddr.itsValue();
}

void UhconnVoxor::run(void) {
    go [=] {
        UhconnMessage in_msg;
        while(true) {
            MsgQ >> in_msg;
            go [=] {
                if(in_msg.getType() == MSG_TYPE_REQ) {
                    #ifdef DEBUGINFO
                    std::cout << "UhconnVoxor::run receive request methode: " << in_msg.getMethodName() << " param:"<< in_msg.getMethodPara() << std::endl;
                    #endif
                    
                    
                    auto methodName = in_msg.getMethodName();
                    auto table = ItsDco->getFunctionTable();
                    auto fun = table.at(methodName);
                    // fun(in_msg.getMethodPara());
                    
                    std::string ret = (ItsDco->*ItsDco->getFunctionTable()[in_msg.getMethodName()])(in_msg.getMethodPara());

                    switch(in_msg.getCmd()) {
                        case MSG_CMD_CREATE:
                        case MSG_CMD_WAIT: 
                        {
                            UhconnMessage out_msg(in_msg.getDestNodeId(), in_msg.getSrcNodeId(), in_msg.getCmd(), MSG_TYPE_RESP);
                            out_msg.setMethodPara(ret);
                            out_msg.setDestVoxor(in_msg.getSrcVoxor());
                            // std::cout << "make response msg" << out_msg.getDestVoxor() << std::endl;
                            UhconnVoxorFactory::getInstance().getLocalWorkNode()->itsRouter().sendMsg(out_msg);
                        }
                        break;

                        case MSG_CMD_ASYNC: 
                        {
                            UhconnDdoRef ddorf(getAddr(), in_msg.getDdoId());
                            UhconnDdo* ddo = new UhconnDdo(ddorf);
                            #ifdef DEBUGINFO
                            std::cout <<"async store data with id:"<<in_msg.getDdoId()<<endl;
                            #endif
                            DdoBlockData d;
                            d.type = 2;
                            d.len = ret.size();
                            d.data = (void*)ret.c_str();
                            ddo->storeBlock(d);
                        }
                        default:
                        
                        break;
                    }
                }
                else {
                    // std::cout << "receive a response!!" << std::endl;
                    switch( in_msg.getCmd()) {
                    case MSG_CMD_CREATE:
                    case MSG_CMD_WAIT: 
                    {
                        ItsDco->getMsgQ() << in_msg;
                    }
                    break;
                    case MSG_CMD_ASYNC:
                    default:
                    break;
                    }
                }
            };
        }
    };
}

co_chan<UhconnMessage>& UhconnVoxor::getMsgQ(void) {
    return MsgQ;
}

void run(void);
