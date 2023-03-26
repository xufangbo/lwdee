
include_directories(.)
include_directories(../uhconnect)
include_directories(../inc/libgo)
include_directories(.../inc/boost)
include_directories(../inc )

aux_source_directory(. DIR)

add_subdirectory(core)
add_subdirectory(lwdee)
add_subdirectory(driver)
add_subdirectory(map)
add_subdirectory(reduce)

# target_link_libraries(uhconnect
#         PUBLIC libgo static_hook uhshell
#         PUBLIC pthread boost_json dl rt
#         )

# target_link_directories(uhconnect
#         PUBLIC ${PROJECT_SOURCE_DIR}/lib/libgo
#         PUBLIC ${PROJECT_SOURCE_DIR}/lib/boost
#         )

# target_compile_definitions(uhconnect PUBLIC DB_BLOCK_WITHOUT_COPY)