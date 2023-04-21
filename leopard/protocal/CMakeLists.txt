aux_source_directory(. DIR)

file(GLOB_RECURSE SRC_protocal "*.cpp")

# ls */*.cpp
add_library(protocal SHARED ${SRC_protocal})

target_link_libraries(net)