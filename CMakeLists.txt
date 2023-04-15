cmake_minimum_required(VERSION 3.10)

project(leopard VERSION 3.0.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -w -ggdb -std=c++2a")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
# add_compile_options(-std=c++11 -fexec-charset=GBK -finput-charset=UTF-8)

set(EXECUTABLE_OUTPUT_PATH ../bin)

aux_source_directory(. DIR)

include_directories(.)
include_directories(./core)
include_directories(./leopard)
include_directories(./dsc)

add_subdirectory(leopard)
add_subdirectory(dsc)

###############################################

add_executable(app dsc/app.cpp dsc/DscDcoFactory.cpp) 

target_link_directories(app  PUBLIC ${PROJECT_SOURCE_DIR}/lib/librdkafka )
target_link_libraries(app PUBLIC rdkafka++)

list(APPEND EXTRA_LIBS dscore) 
list(APPEND EXTRA_LIBS dscore) 
list(APPEND EXTRA_LIBS leopard) 
list(APPEND EXTRA_LIBS matrix) 
list(APPEND EXTRA_LIBS kafka) 
list(APPEND EXTRA_LIBS map) 
list(APPEND EXTRA_LIBS reduce) 
list(APPEND EXTRA_LIBS driver)
target_link_libraries(app PUBLIC ${EXTRA_LIBS})

target_compile_options(app PUBLIC -Wall)

