include_directories(${PROJECT_SOURCE_DIR}/core)

file(GLOB_RECURSE SRC_core "core/*.cpp")
file(GLOB_RECURSE SRC_net "net/*.cpp")
file(GLOB_RECURSE SRC_protocal "protocal/*.cpp")
file(GLOB_RECURSE SRC_antelope "antelope/*.cpp")
file(GLOB_RECURSE SRC_leopard "leopard/*.cpp")

add_library(leopard  ${SRC_core} ${SRC_net} ${SRC_protocal} ${SRC_antelope} ${SRC_leopard})
target_link_libraries(leopard pthread)

add_executable(server ${SRC_core} ${SRC_net} ${SRC_protocal} ${SRC_leopard} leopard/app.cc)
target_link_libraries(server core pthread)

add_executable(client ${SRC_core} ${SRC_net} ${SRC_protocal} ${SRC_antelope} antelope/app.cc)
target_link_libraries(client core pthread)