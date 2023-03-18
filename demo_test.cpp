/**
 * @file simple_run.cpp
 * @author your name (shiqingpeng@domain.com)
 * @brief  对UhomeConnect组件接口的基本调用演示
 * @version 0.1
 * @date 2022-08-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "core/UhconnVoxorFactory.h"
#include "UserDco.h"
#include "api/UhconnApi.h"
#include "api/UhconnDcoRef.h"
#include "core/UhconnRouter.h"
#include "core/UhconnWorkNode.h"
#include <iostream>
#include <string>
#include <libgo.h>
#include "UserDcoFactory.h"
#include "api/UhconnDdo.h"
#include "api/UhconnDcoRef.h"
#include "api/UhconnDdoRef.h"
#include "shell_cpp.h"
#include "core/UhconnSimpleAddr.h"
#include <chrono>
#include "Benchmark.h"

int simple_main(void) {
    
    std::cout << "simple test start!" << std::endl;
    //创建用户DCO
    UhconnDcoRef dcoA = UhconnApi::create(std::string("UserDco"));

    UhconnDdoRef df(UhconnVoxorFactory::getInstance().getLocalWorkNode()->itsDeamon().itsAddr());
    UhconnDdo para(df);
    DdoBlockData test_data;
    test_data.type = 22;
    test_data.len = 10;
    test_data.data = new char[10]; 
    memcpy(test_data.data, "test data", 10);
    para.storeBlock(test_data);
    delete [](char*)test_data.data;
    UhconnDdoRef ddoref = UhconnApi::async(dcoA, std::string("f1"), &para);

    UhconnDdo* ddo = UhconnApi::wait(ddoref);

    DdoBlockData result;
    ddo->loadBlock(result);
    std::cout << "result len:" << result.len << "type:" << result.type << std::endl; 
    std::cout << "simple test completed!" << std::endl;
    return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), simplrun, simple_main, run test);


int ddo_make(size_t n_Mbytes) {
    auto time_make = new Benchmark("make");
    UhconnDdoRef df(UhconnVoxorFactory::getInstance().getLocalWorkNode()->itsDeamon().itsAddr());
    UhconnDdo* large_ddo = new UhconnDdo(df);
    DdoBlockData large_data;
    large_data.len = n_Mbytes*1024*1024;
    large_data.data = new int[large_data.len/4];
    if(!large_data.data) {
        return -1;
    }
    int* intp = (int*)(large_data.data);
    for (size_t i = 0; i < large_data.len/4; i++)
    {
        intp[i] = i;
    }
    large_ddo->storeBlock(large_data);
    delete [](int*)large_data.data;
    std::cout << 
        "make ddo: " << large_ddo->itsRef().itsId() << 
        " |size:" << large_data.len << 
        " |at:" << large_ddo->itsRef().itsVoxorId() << 
        std::endl;

    delete time_make;
    return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), ddomk, ddo_make, make ddo);

int ddo_copy(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Usage: ddocp <w> <v> <d>" << std::endl;
        std::cout << "   w is the sn of worknode"
        << std::endl;
        std::cout << "   v is the sn of voxor."
        << std::endl;
        std::cout << "   d is the ddo id."
        << std::endl;
        return -1;
    }    
    int wnid = atoi(argv[1]);
    int vid = atoi(argv[2]);
    DdoDataId did = atoll(argv[3]);
    UhconnSimpleAddr addr(wnid, vid);
    UhconnDdoRef df(addr.itsValue(), did);
    UhconnDdo* large_ddo = new UhconnDdo(df);
    std::cout << "copy ddo: " << large_ddo->itsRef().itsId() << " |at:" << large_ddo->itsRef().itsVoxorId() << std::endl;
    DdoBlockData result;
    {
        Benchmark bm("load:");
        large_ddo->loadBlock(result);
    }
    uint32_t* intp = (uint32_t*)(result.data);
    for (size_t i = 0; i < result.len/4; i++)
    {
        if( intp[i] != i ) {
            std::cout << "!!!Data Error!!!" << intp[i] << "|" << i << std::endl;
            break;
        } 
    }
    
    return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), ddocp, ddo_copy, copy ddo);


int ddo_write(size_t n_Mbytes) {
    UhconnDdoRef df(UhconnVoxorFactory::getInstance().getLocalWorkNode()->itsDeamon().itsAddr());
    UhconnDdo* large_ddo = new UhconnDdo(df);
    DdoBlockData large_data;
    large_data.len = n_Mbytes*1024*1024;
    large_data.data = new int[large_data.len/4];
    if(!large_data.data) {
        return -1;
    }
    int* intp = (int*)(large_data.data);
    for (size_t i = 0; i < large_data.len/4; i++)
    {
        intp[i] = i;
    }
    large_ddo->storeBlock(large_data);
    delete [](int*)large_data.data;
    std::cout << 
        "make ddo: " << large_ddo->itsRef().itsId() << 
        " |size:" << large_data.len << 
        " |at:" << large_ddo->itsRef().itsVoxorId() << 
        std::endl;
    return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), ddowrt, ddo_write, make ddo);