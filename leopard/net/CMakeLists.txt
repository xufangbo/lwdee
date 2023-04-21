aux_source_directory(. DIR)

file(GLOB_RECURSE SRC_net "*.cpp")

# ls */*.cpp
add_library(net SHARED ${SRC_net})

target_link_libraries(core)