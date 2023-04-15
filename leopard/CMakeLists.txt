cmake_minimum_required(VERSION 3.10)

project(leopard VERSION 3.0.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -w -ggdb -std=c++2a")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
# add_compile_options(-std=c++11 -fexec-charset=GBK -finput-charset=UTF-8)

######################################################
# find_package(open62541 REQUIRED)

# message("hello ${open62541_TOOLS_DIR}")
# include_directories("${open62541_TOOLS_DIR}/../../../include")
######################################################

set(EXECUTABLE_OUTPUT_PATH ../bin)
# set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(.)

aux_source_directory(src DIR)

add_subdirectory(core)
add_subdirectory(net)
# add_subdirectory(server)
# add_subdirectory(client)


file(GLOB_RECURSE SRC_core "core/*.cpp")
file(GLOB_RECURSE SRC_net "net/*.cpp")
file(GLOB_RECURSE SRC_protocal "protocal/*.cpp")
file(GLOB_RECURSE SRC_client "client/*.cpp")
file(GLOB_RECURSE SRC_server "server/*.cpp")

# message(STATUS "ready to remove ${PROJECT_SOURCE_DIR}/net/ProtocalFactory.cpp")
# list(REMOVE_ITEM ${SRC_net} "${PROJECT_SOURCE_DIR}/net/ProtocalFactory.cpp")
# list(REMOVE_ITEM ${SRC_net} /home/ubuntu/gitee/leopard/net/ProtocalFactory.cpp)

# foreach(x ${SRC_net})
#     message(STATUS "item is ${x}")
# endforeach()

# add_executable(client 
# client/client.cpp
# client/SocketClient.cpp
# )
# target_link_libraries(client core net pthread)

add_executable(client  ${SRC_core} ${SRC_net} ${SRC_protocal} ${SRC_client})
target_link_libraries(client pthread)

# add_executable(app 
# server/app.cpp
# server/Leopard.cpp
# server/Runway.cpp
# )
# target_link_libraries(app  core net pthread)   

add_executable(app  ${SRC_core} ${SRC_net} ${SRC_protocal} ${SRC_server})
target_link_libraries(app pthread)   