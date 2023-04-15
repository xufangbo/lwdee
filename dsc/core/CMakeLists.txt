
file(GLOB_RECURSE SRC_src "*.cpp")

add_library (dscore ${SRC_src})

target_link_libraries(dscore pthread)