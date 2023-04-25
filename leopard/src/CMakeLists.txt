aux_source_directory(. DIR)

file(GLOB_RECURSE SRC_src "*.cpp")

# ls */*.cpp
add_library(leopard  ${SRC_src}) 

target_link_libraries(leopard core)