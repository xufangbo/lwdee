
# file(GLOB_RECURSE kmeans_src ./*.cpp)
# add_library(kmeans ${kmeans_src})
# target_link_libraries(kmeans
#         PUBLIC uhconnect
#         )
# target_include_directories(kmeans 
#         PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
#         )
# target_link_directories(kmeans
#         PUBLIC ${PROJECT_SOURCE_DIR}/lib/boost
#         )
# target_compile_options(kmeans PRIVATE "-O2")    

include_directories(.)
include_directories(../uhconnect)

add_executable(app app.cpp)