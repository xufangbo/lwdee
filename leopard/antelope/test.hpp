#pragma once

#include <functional>
#include <iostream>
#include "LaneClient.hpp"

std::string input_small(int i);
std::string input_large(int i);

// RequestCallback callback;

typedef std::function<std::string(int i)> InputType;

void test_short_sync(int testSize, InputType inputType, std::string ip, int port);
void test_short_async(int testSize, InputType inputType, std::string ip, int port);

void test_long_sync(int testSize, InputType inputType,int parallel, std::string ip, int port);
void test_long_async(int testSize, InputType inputType,int parallel, std::string ip, int port);

/**
 *
 *  LT elapsed 10.440
 * ( 1681957318289 -> 1 -> 1 -> 1 ) <==> ( 0.001 + 0.000 + 0.000 = 0.001 )
 *
 * ET elapsed 10.679
 *  ( 1681957778783 -> 2 -> 2 -> 3 ) <==> ( 0.002 + 0.000 + 0.001 = 0.003 )
 */
// void test_1000_small_short_sync(std::string ip, int port);

/**
 *
 * LT elapsed 0.371
 * ( 1681958075938 -> 95 -> 95 -> 108 ) <==> ( 0.095 + 0.000 + 0.013 = 0.108 )
 *
 *
 * ET elapsed  43.046
 *   ( 1681957918673 -> 27 -> 29 -> 30 ) <==> ( 0.027 + 0.002 + 0.001 = 0.030 )
 * 1. 客户端频繁出现 long time to connect: 1.030000s
 * 2.  wait : [588] - timeout
 */
// void test_1000_small_short_async(std::string ip, int port);

/**
 *
 * LT elapsed 51.310
 * ( 1681958011470 -> 46 -> 48 -> 49 ) <==> ( 0.046 + 0.002 + 0.001 = 0.049 )
 *
 *
 * ET elapsed  43.046
 *   ( 1681957918673 -> 27 -> 29 -> 30 ) <==> ( 0.027 + 0.002 + 0.001 = 0.030 )
 */
// void test_1000_large_short_sync(std::string ip, int port);

/**
 * LT : elapsed 21.117
 *   ( 1681959366625 -> 7421 -> 7424 -> 7424 ) <==> ( 7.421 + 0.003 + 0.000 = 7.424 )
 *
 * ET : 执行失败，客户端能收到第600个回复
 * 1. 后面的连接： ( 1681959501965 -> 125914 -> 125917 -> 125918 ) <==> ( 125.914 + 0.003 + 0.001 = 125.918 )
 * 2. 客户端频繁出现 long time to connect: 1.030000s
 * 2.  wait : [567] - timeout
 */
// void test_1000_large_short_async(std::string ip, int port);
