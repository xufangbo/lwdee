aux_source_directory(. DIR)

file(GLOB_RECURSE SRC_protocal "*.cpp")

# ls */*.cpp
add_library(protocal ${SRC_protocal})

target_link_libraries(protocal core net)