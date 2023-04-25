include_directories(${PROJECT_SOURCE_DIR}/core)

add_subdirectory(src)
# add_subdirectory(protocal)

file(GLOB_RECURSE SRC_src "src/*.cpp")
file(GLOB_RECURSE SRC_prot "src/prot/*.cpp")
file(GLOB_RECURSE SRC_client "test-client/*.cpp")
file(GLOB_RECURSE SRC_server "test-server/*.cpp")

# add_library(leopard ${SRC_client} ${SRC_server})
# target_link_libraries(leopard pthread core leopard protocal)

# add_executable(server ${SRC_server} leopard/app.cc)
# target_link_libraries(server pthread core leopard protocal)

# add_executable(client ${SRC_client} antelope/app.cc)
# target_link_libraries(client pthread core leopard protocal)

add_executable(server ${SRC_src} ${SRC_prot} ${SRC_server})
target_link_libraries(server pthread core)

add_executable(client  ${SRC_src} ${SRC_prot} ${SRC_client})
target_link_libraries(client pthread core)