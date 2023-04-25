#pragma once

#include <iostream>
#include "test.hpp"

void test_input(std::string ip, int port);
static void test_execute(Testor* testor, TestInput& input, int testSize, int parallel, float timeout, std::string ip, int port);
static void test_by_testor(Testor* testor, int testSize, int parallel, std::string ip, int port);