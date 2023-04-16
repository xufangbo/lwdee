include_directories(./dsc)
include_directories(./inc)

add_subdirectory(dsc)

add_executable(app dsc/app.cpp) 

target_link_directories(app  PUBLIC ${PROJECT_SOURCE_DIR}/lib/librdkafka )
target_link_libraries(app PUBLIC rdkafka++)

list(APPEND EXTRA_LIBS core) 
list(APPEND EXTRA_LIBS dscore) 
list(APPEND EXTRA_LIBS leopard) 
list(APPEND EXTRA_LIBS matrix) 
list(APPEND EXTRA_LIBS kafka) 
list(APPEND EXTRA_LIBS map) 
list(APPEND EXTRA_LIBS reduce) 
list(APPEND EXTRA_LIBS driver)
target_link_libraries(app PUBLIC ${EXTRA_LIBS})

# target_compile_options(app PUBLIC -Wall)

##################################
add_executable(bishou bishou.cpp) 
target_link_libraries(bishou pthread)