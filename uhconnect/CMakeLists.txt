
file(GLOB_RECURSE connect_src ./*.cpp)
add_library(uhconnect ${connect_src})

target_link_libraries(uhconnect
        PUBLIC libgo static_hook uhshell
        PUBLIC pthread boost_json dl rt
        )

target_include_directories(uhconnect 
        PUBLIC ${PROJECT_SOURCE_DIR}/inc/libgo
        PUBLIC ${PROJECT_SOURCE_DIR}/inc/boost
        PUBLIC ${PROJECT_SOURCE_DIR}/inc
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_directories(uhconnect
        PUBLIC ${PROJECT_SOURCE_DIR}/lib/libgo
        PUBLIC ${PROJECT_SOURCE_DIR}/lib/boost
        )

target_compile_definitions(uhconnect PUBLIC DB_BLOCK_WITHOUT_COPY)