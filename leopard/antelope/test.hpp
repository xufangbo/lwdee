#pragma once

#include <iostream>
#include "SocketClient.hpp"

/**
 * @brief 执行正常，耗时10s
 *
 * @param ip
 * @param port
 */
void test_1000_small_short_sync(std::string ip, int port);

/**
 * 达到800个请求之后，服务端就收不到数据了，没有更新的
 * 单个请求时间越来越长，第800个请求达到42s
 * 800个之后waiter中time out
 * 主要的原因还是客户端的socket没有及时closed
 * 客户端1000个请求都发送完了
 *
 * ET -> LT 3.09s
 * https://blog.csdn.net/CODINGCS/article/details/115046256
 * 
 * --------------------
 * 有时候客户端会有Segmentation fault
 * 2023-04-19 21:24:27,103 [error] [139858379597632] /home/kevin/git/lwdee/leopard/antelope/Lane.cpp:97 close : can't find fd 10
 *
 *
 */
void test_1000_small_short_async(std::string ip, int port);

/**
 * total 300s per 0.3s
 *  ( 1681919147995 -> 302 -> 305 -> 306 ) <==> ( 0.302 + 0.003 + 0.001 = 0.306 )
 */
void test_1000_large_short_sync(std::string ip, int port);

/**
 * 
 * Segmentation fault (core dumped)  /home/kevin/git/lwdee/leopard/net/SendTaskQueue.cpp:21
 */
void test_1000_large_short_async(std::string ip, int port);

void testLongConnection(SocketClient* client, int i);