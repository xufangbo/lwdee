include_directories(${PROJECT_SOURCE_DIR}/core)

add_subdirectory(net)
add_subdirectory(protocal)

file(GLOB_RECURSE SRC_antelope "antelope/*.cpp")
file(GLOB_RECURSE SRC_leopard "leopard/*.cpp")

add_library(leopard ${SRC_antelope} ${SRC_leopard})
target_link_libraries(leopard net pthread)

add_executable(server ${SRC_leopard} leopard/app.cc)
target_link_libraries(server net core pthread protocal)

add_executable(client ${SRC_antelope} antelope/app.cc)
target_link_libraries(client net core pthread protocal)