include_directories(${PROJECT_SOURCE_DIR}/core)

add_subdirectory(net)
add_subdirectory(protocal)

file(GLOB_RECURSE SRC_net "net/*.cpp")
file(GLOB_RECURSE SRC_protocal "protocal/*.cpp")
file(GLOB_RECURSE SRC_antelope "antelope/*.cpp")
file(GLOB_RECURSE SRC_leopard "leopard/*.cpp")

# add_library(leopard ${SRC_antelope} ${SRC_leopard})
# target_link_libraries(leopard pthread core net protocal)

# add_executable(server ${SRC_leopard} leopard/app.cc)
# target_link_libraries(server pthread core net protocal)

# add_executable(client ${SRC_antelope} antelope/app.cc)
# target_link_libraries(client pthread core net protocal)

add_executable(server ${SRC_net} ${SRC_protocal} ${SRC_leopard} leopard/app.cc)
target_link_libraries(server pthread core net protocal)

add_executable(client  ${SRC_net} ${SRC_protocal} ${SRC_antelope} antelope/app.cc)
target_link_libraries(client pthread core net protocal)