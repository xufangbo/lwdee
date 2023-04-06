
include_directories(./dsc)
include_directories(./uhconnect)

add_subdirectory(uhconnect)
add_subdirectory(uhshell)
add_subdirectory(dsc)

add_executable(app dsc/app.cpp dsc/DscDcoFactory.cpp) 

target_link_directories(app
        PUBLIC ${PROJECT_SOURCE_DIR}/lib/librdkafka
        )
target_link_libraries(app PUBLIC rdkafka++)

# list(APPEND EXTRA_LIBS rdkafka++)
# list(APPEND EXTRA_LIBS pthread)

list(APPEND EXTRA_LIBS core) 
list(APPEND EXTRA_LIBS matrix) 
list(APPEND EXTRA_LIBS lwdee) 
list(APPEND EXTRA_LIBS kafka) 
list(APPEND EXTRA_LIBS map) 
list(APPEND EXTRA_LIBS reduce) 
list(APPEND EXTRA_LIBS driver)
target_link_libraries(app PUBLIC ${EXTRA_LIBS})

target_link_options(app PUBLIC "-T${PROJECT_SOURCE_DIR}/app.lds" )

target_compile_options(app PUBLIC -Wall)
