include_directories(${PROJECT_SOURCE_DIR}/core)

file(GLOB_RECURSE SRC_core "core/*.cpp")
file(GLOB_RECURSE SRC_net "net/*.cpp")
file(GLOB_RECURSE SRC_protocal "protocal/*.cpp")
file(GLOB_RECURSE SRC_client "client/*.cpp")
file(GLOB_RECURSE SRC_server "server/*.cpp")

add_library(leopard  ${SRC_core} ${SRC_net} ${SRC_protocal} ${SRC_client} ${SRC_server})
target_link_libraries(leopard pthread)   