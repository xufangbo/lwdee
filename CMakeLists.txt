cmake_minimum_required(VERSION 3.10)

project(leopard VERSION 3.0.0.0 LANGUAGES CXX C)

# set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -w -ggdb")

# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -w -ggdb -std=c++2a")
# add_compile_options(-std=c++11 -fexec-charset=GBK -finput-charset=UTF-8)

# add_compile_definitions(LEOPARD_SUSPEND)
# add_compile_definitions(LEOPARD_TRACING)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(. DIR)

include_directories(.)
include_directories(./core)
include_directories(./leopard)

add_subdirectory(core)
add_subdirectory(leopard)

# include(${CMAKE_CURRENT_SOURCE_DIR}/dsc.cmake)

