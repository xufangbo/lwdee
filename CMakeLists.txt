# 设置运行此配置文件所需的CMake最低版本
cmake_minimum_required(VERSION 3.12)

# set the project name and version
# 设置项目名称和版本
project(Demo VERSION 1.0.0)

# specify the C++ standard
# 指定C ++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -w -g -ggdb")
# set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -w -g")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# include(${CMAKE_CURRENT_SOURCE_DIR}/demo.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/terasort.cmake)
