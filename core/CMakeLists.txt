aux_source_directory(. DIR)

file(GLOB_RECURSE SRC_src "./*.cpp")

add_library (core  ${SRC_src})

target_link_libraries(core pthread)